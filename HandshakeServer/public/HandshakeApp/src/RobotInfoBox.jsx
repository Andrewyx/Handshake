// Import the functions you need from the SDKs you need
// import { initializeApp } from "firebase/app";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, get } from 'firebase/database';
import { useEffect, useState } from "react";
import ValueIndicator from "./ValueIndicator";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

export default function RobotInfoBox() {
  // Your web app's Firebase configuration
  const firebaseConfig = {
    apiKey: "AIzaSyDqATxGy0ATTYjuo-K6fOB9AUAG1AWzspU",
    authDomain: "handshake-664b7.firebaseapp.com",
    databaseURL: "https://handshake-664b7-default-rtdb.firebaseio.com",
    projectId: "handshake-664b7",
    storageBucket: "handshake-664b7.appspot.com",
    messagingSenderId: "756903976373",
    appId: "1:756903976373:web:9defddd5cbed24fb5d8dfc"
  };
  // Initialize Firebase
  const app = initializeApp(firebaseConfig);
  const db = getDatabase(app);
  // Database Paths

  let dataLeftMotorPath = 'robots/leftmotor';
  let dataRightMotorPath = 'robots/rightmotor';

  // Get a database reference 
  const databaseLeftMotor = ref(db, dataLeftMotorPath);
  const databaseRightMotor = ref(db, dataRightMotorPath);


  // letiables to save database current values
  let [leftMotorReading, setLeftMotorReading] = useState([]);
  let [rightMotorReading, setRightMotorReading] = useState([]);

  useEffect(() => {
    // Attach an asynchronous callback to read the data
    onValue(databaseLeftMotor, (snapshot) => {
      setLeftMotorReading(snapshot.val());
      console.log(leftMotorReading);
      }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    });

    onValue(databaseRightMotor, (snapshot) => {
      setRightMotorReading(snapshot.val());
      console.log(rightMotorReading);
      }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    });
  }, [])


  return(
    <>
        <div>
          <ValueIndicator name='Left Motor Value' value={leftMotorReading} />
          <ValueIndicator name='Right Motor Value' value={rightMotorReading} />
        </div>
    </>
  )}
