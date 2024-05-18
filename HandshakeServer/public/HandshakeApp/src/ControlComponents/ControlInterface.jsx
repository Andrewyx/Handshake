import { child, get, onValue, ref, update } from 'firebase/database';
import { useEffect, useState } from 'react';
import { softwareConstants } from '../Util/util';
import ControlButton from './ControlButton'
import RobotInfoBox from './RobotInfoBox'
import FirebaseTools from '../Util/FirebaseTools';
import MessageBox from './MessageBox';

const FIREBASE = FirebaseTools.getInstance();

export default function ControlInterface() {
    let [connectedRobotID, setRobotConnectionID] = useState(null);
    let [robotStatus, setRobotStatus] = useState(false);
  
    useEffect(() => {
  
      onValue(ref(FIREBASE.db, `${FIREBASE.getUserPath()}/robotID`), (snapshot) => {
        setRobotConnectionID(snapshot.val());
      });
      if (connectedRobotID) {
        get(child(ref(FIREBASE.db), `robotIDs/${connectedRobotID}/lastactive`)).then((snapshot) => {
          updateRobotStatus(snapshot.val());
        });
        onValue(ref(FIREBASE.db, `robotIDs/${connectedRobotID}/lastactive`), (snapshot) => {
          updateRobotStatus(snapshot.val());
        });
      }
    })
  
    function updateRobotStatus(value) {
      if (Date.now() * softwareConstants.MILLISECONDS_TO_SECONDS
        - value <= softwareConstants.ROBOT_TIMEOUT_IN_SECONDS) {
        setRobotStatus(true);
      } else {
        setRobotStatus(false);
      }
    }  
  
    function setAndUpdateRobotID(id) {
      const updates = {};
      updates[`${FIREBASE.getUserPath()}/robotID`] = id;
      update(ref(FIREBASE.db), updates);
      FIREBASE.connectedRobotID = id;
    }
  
    function isRobotIDValid(e) {
      e.preventDefault();
  
      const formData = new FormData(e.target);
      const formJson = Object.fromEntries(formData.entries());


      if (typeof formJson.id !== "string") {
        console.log("Invalid ID Type");
        return false;
      }

      const input = formJson.id.toUpperCase();

      get(child(ref(FIREBASE.db), `robotIDs/${input}`)).then((snapshot) => {
        if (snapshot.exists()) {
          setAndUpdateRobotID(input);
          setRobotConnectionID(true);
        } else {
          console.log("Invalid ID");
        }
      }).catch((error) => {
        console.error(error);
      });
    }
  
    if (connectedRobotID) {
      return (
        <>
          <div id='statusLabels'>
            <div className='statusLabel'>
              <label>Robot {connectedRobotID}</label>
            </div>
            <div className='statusLabel'>
              <label>Status: </label>
              {robotStatus ? <label id="on">Online</label> : <label id="off">Offline</label>}
            </div>
          </div>
            <div className='controlInterface'>
              <ControlButton name="Drive Left" value={120} path={FIREBASE.dataLeftMotorPath} />
              <ControlButton name="Drive Right" value={120} path={FIREBASE.dataRightMotorPath} />
            </div>
            <MessageBox />
          <button onClick={() => { setAndUpdateRobotID(null) }}>Disconnect</button>
        </>
      )
    }
    else {
      return (
        <form onSubmit={isRobotIDValid}>
          <input id="robotID" name="id" placeholder="Enter Robot ID to Connect" />
          <button type="submit">Connect</button>
        </form>
      )
    }
  }