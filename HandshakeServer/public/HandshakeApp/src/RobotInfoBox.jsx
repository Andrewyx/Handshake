// Import the functions you need from the SDKs you need
import { ref, onValue, child } from 'firebase/database';
import { useEffect, useState } from "react";
import ValueIndicator from "./ValueIndicator";
import FirebaseTools from './FirebaseTools';

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

export default function RobotInfoBox() {
  const FIREBASE = FirebaseTools.getInstance();
  const DEFAULT_VALUE = [];

  // letiables to save database current values
  let [leftMotorReading, setLeftMotorReading] = useState(DEFAULT_VALUE);
  let [rightMotorReading, setRightMotorReading] = useState(DEFAULT_VALUE);

  useEffect(() => {
    // Attach an asynchronous callback to read the data
    onValue(ref(FIREBASE.db, FIREBASE.dataLeftMotorPath), (snapshot) => {
      setLeftMotorReading(snapshot.val());
      !Object.is(leftMotorReading, DEFAULT_VALUE) ? console.log(leftMotorReading) : null;
      }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    });

    onValue(ref(FIREBASE.db, FIREBASE.dataRightMotorPath), (snapshot) => {
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
