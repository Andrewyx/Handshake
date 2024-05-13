import './App.css'

import FirebaseTools from './FirebaseTools';
import { SignInButton, SignOutButton } from './AuthComponents';
import { useAuthState } from 'react-firebase-hooks/auth';
import ControlInterface from './ControlInterface';

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

export default App