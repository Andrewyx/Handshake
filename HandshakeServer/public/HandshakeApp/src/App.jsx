import RobotInfoBox from './RobotInfoBox'
import './App.css'
import ControlButton from './ControlButton'
import FirebaseTools from './FirebaseTools';
import { ref } from 'firebase/database';

function App() {

  const FIREBASE = FirebaseTools.getInstance();
  let dataLeftMotorPath = 'robots/leftmotor';
  let dataRightMotorPath = 'robots/rightmotor';

  // Get a database reference 
  const databaseLeftMotor = ref(FIREBASE.db, dataLeftMotorPath);
  const databaseRightMotor = ref(FIREBASE.db, dataRightMotorPath);

  return (
    <>
      <div>
      </div>
      <h1>Handshake Control Interface</h1>
      <div className="card">

        <p>
        Future controls for <code>handy buddies</code> to be placed here!
        </p>
        <ControlButton name="Drive Left" value={120} pathRef={databaseLeftMotor}/>
        <ControlButton name="Drive Right" value={120} pathRef={databaseRightMotor}/>
      </div>
      <RobotInfoBox />
    </>
  )
}

export default App
