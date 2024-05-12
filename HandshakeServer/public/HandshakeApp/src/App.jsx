import RobotInfoBox from './RobotInfoBox'
import './App.css'
import ControlButton from './ControlButton'
import FirebaseTools from './FirebaseTools';
import { SignInButton, SignOutButton } from './AuthComponents';
import { useAuthState } from 'react-firebase-hooks/auth';
import { child, get, onValue, ref, update } from 'firebase/database';
import { useEffect, useState } from 'react';

const FIREBASE = FirebaseTools.getInstance();

function App() {

  const auth = FIREBASE.auth;
  auth.useDeviceLanguage();
  auth.languageCode = 'it';
  const [user] = useAuthState(auth);

  return (
    <>
      <div>
      </div>
      <h1>Handshake Control Interface</h1>
      <div className="card">
        <p>
          Future controls for <code>handy buddies</code> to be placed here!
        </p>
        {user ? <ControlInterface /> : <SignInButton />}
      </div>
      {!user ? <></> : <SignOutButton />}
    </>
  )
}

function ControlInterface() {
  let [connectedRobotID, setRobotConnectionID] = useState(null);
  let [robotStatus, setRobotStatus] = useState(false);

  useEffect(() => {
    get(child(ref(FIREBASE.db), `${FIREBASE.getUserPath()}/robotID`))
      .then((snapshot) => {
        if (snapshot.exists() && snapshot.val() >= 0) {
          setRobotConnectionID(snapshot.val());
        }
      })
    if (connectedRobotID) {
      onValue(ref(FIREBASE.db, `robotIDs/robot${connectedRobotID}`), (snapshot) => {
        setRobotStatus(snapshot.val());
        }, (errorObject) => {
        console.log('The read failed: ' + errorObject.name);
      });
    }
  })

  function isRobotIDValid(e) {
    e.preventDefault();

    const formData = new FormData(e.target);
    const formJson = Object.fromEntries(formData.entries());

    if (typeof parseInt(formJson.id) !== "number" || parseInt(formJson.id) < 0) {
      console.log("Invalid ID Type");
      return false;
    }

    const id = parseInt(formJson.id);

    get(child(ref(FIREBASE.db), `robotIDs/robot${id}`)).then((snapshot) => {
      if (snapshot.exists()) {
        const updates = {};
        updates[`${FIREBASE.getUserPath()}/robotID`] = id;

        update(ref(FIREBASE.db), updates);
        FIREBASE.connectedRobotID = id;
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
        <button>Disconnect</button>
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

export default App