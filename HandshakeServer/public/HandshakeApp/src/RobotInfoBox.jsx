// Import the functions you need from the SDKs you need
import { ref, onValue } from 'firebase/database';
import { useEffect, useState } from "react";
import ValueIndicator from "./ValueIndicator";
import FirebaseTools from './FirebaseTools';

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

export default function RobotInfoBox(props) {
  const FIREBASE = FirebaseTools.getInstance();
  const DEFAULT_VALUE = [];
  let dataLeftMotorPath = 'robots/leftmotor';
  let dataRightMotorPath = 'robots/rightmotor';

  // Get a database reference 
  const databaseLeftMotor = ref(FIREBASE.db, dataLeftMotorPath);
  const databaseRightMotor = ref(FIREBASE.db, dataRightMotorPath);


  // letiables to save database current values
  let [leftMotorReading, setLeftMotorReading] = useState(DEFAULT_VALUE);
  let [rightMotorReading, setRightMotorReading] = useState(DEFAULT_VALUE);

  useEffect(() => {
    // Attach an asynchronous callback to read the data
    onValue(databaseLeftMotor, (snapshot) => {
      setLeftMotorReading(snapshot.val());
      !Object.is(leftMotorReading, DEFAULT_VALUE) ? console.log(leftMotorReading) : null;
      }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    });

    onValue(databaseRightMotor, (snapshot) => {
      setRightMotorReading(snapshot.val());
      !Object.is(rightMotorReading, DEFAULT_VALUE) ? console.log(rightMotorReading) : null;
      }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    });
  }, DEFAULT_VALUE)


  return(
    <>
        <div>
          <ValueIndicator name='Left Motor Value' value={leftMotorReading} />
          <ValueIndicator name='Right Motor Value' value={rightMotorReading} />
        </div>
    </>
  )}
