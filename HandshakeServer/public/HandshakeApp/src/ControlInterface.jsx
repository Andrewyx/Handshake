import { child, get, onValue, ref, update } from 'firebase/database';
import { useEffect, useState } from 'react';
import { softwareConstants } from './util';
import ControlButton from './ControlButton'
import RobotInfoBox from './RobotInfoBox'
import FirebaseTools from './FirebaseTools';

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
  
      get(child(ref(FIREBASE.db), `robotIDs/${formJson.id}`)).then((snapshot) => {
        if (snapshot.exists()) {
          setAndUpdateRobotID(formJson.id);
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
        <div>
          <div>
            <label>Robot {connectedRobotID}</label> <br />
            <label>Status: </label>
            {robotStatus ? <label id="on">Online</label> : <label id="off">Offline</label>}
          </div>
          <ControlButton name="Drive Left" value={120} path={FIREBASE.dataLeftMotorPath} />
          <ControlButton name="Drive Right" value={120} path={FIREBASE.dataRightMotorPath} />
          <RobotInfoBox />
          <button onClick={() => { setAndUpdateRobotID(null) }}>Disconnect</button>
        </div>
      )
    }
    else {
      return (
        <form onSubmit={isRobotIDValid}>
          <input name="id" placeholder="Enter Robot ID to Connect" />
          <button type="submit">Connect</button>
        </form>
      )
    }
  }