import './App.css'

import FirebaseTools from './Util/FirebaseTools';
import { SignInButton, SignOutButton } from './Util/AuthComponents';
import { useAuthState } from 'react-firebase-hooks/auth';
import ControlInterface from './ControlComponents/ControlInterface';

const FIREBASE = FirebaseTools.getInstance();

function App() {

  const [user] = useAuthState(FIREBASE.auth);

  return (
    <>
      <div>
      </div>
      <h1>Handshake Control Interface</h1>
      <div className="card">
        {user ? <ControlInterface /> : <SignInButton />}
      </div>
      {!user ? <></> : <SignOutButton />}
    </>
  )
}

export default App