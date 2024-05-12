import { initializeApp } from "firebase/app";
import { child, get, getDatabase, ref, set } from 'firebase/database';
import { GoogleAuthProvider, getAuth, onAuthStateChanged, signInWithPopup, signOut} from 'firebase/auth';
import { hardwareConstants } from "./util";

export default class FirebaseTools {
    // Initialize Firebase
    static #instance;
    app;
    db;
    auth;
    provider;
    dataLeftMotorPath; 
    dataRightMotorPath;
    connectedRobotID;
    motorValues = {};
    #uid;
    // Your web app's Firebase configuration
    firebaseConfig = {
        apiKey: "AIzaSyDqATxGy0ATTYjuo-K6fOB9AUAG1AWzspU",
        authDomain: "handshake-664b7.firebaseapp.com",
        databaseURL: "https://handshake-664b7-default-rtdb.firebaseio.com",
        projectId: "handshake-664b7",
        storageBucket: "handshake-664b7.appspot.com",
        messagingSenderId: "756903976373",
        appId: "1:756903976373:web:9defddd5cbed24fb5d8dfc"
    };
    constructor() {
        this.app = initializeApp(this.firebaseConfig);
        this.db = getDatabase(this.app);
        this.auth = getAuth();
        this.#uid = '';
        onAuthStateChanged(this.auth, (user) => {
            if (user) {
                this.#uid = user.uid;
                this.#fetchMotorPaths();
            }
        });
        this.provider = new GoogleAuthProvider();
        this.motorValues = {
            leftMotor: hardwareConstants.PWM_STOP_VALUE,
            rightMotor: hardwareConstants.PWM_STOP_VALUE, 
        }
    }

    static getInstance() {
        if (this.#instance) {
            return this.#instance;
        } else {
            this.#instance = new FirebaseTools();
            return this.#instance;
        }
    }

    isUserLoggedIn() {
        if ( this.auth.currentUser != null) {
            this.#uid = this.auth.currentUser.uid;
            return true;
        } 
        else {
            return false;
        }
    }

    login() {
        signInWithPopup(this.auth, this.provider)
        .then((result) => {
            const credential = GoogleAuthProvider.credentialFromResult(result);
            const token = credential.accessToken;
            const user = result.user;
            this.#uid = user.uid;
            get(child(ref(this.db), `users/${this.#uid}`)).then((snapshot) => {
            if (!snapshot.exists()) {
                set(ref(this.db, `users/${this.#uid}`), {
                    leftmotor: hardwareConstants.PWM_STOP_VALUE,
                    rightmotor: hardwareConstants.PWM_STOP_VALUE,
                    robotID : -1
                    });
            } 
            this.dataLeftMotorPath = `users/${this.#uid}/leftmotor`;
            this.dataRightMotorPath = `users/${this.#uid}/rightmotor`;
            this.connectedRobotID = `users/${this.#uid}/robotID`;
            this.#fetchMotorsValues();
            this.#fetchMotorPaths();
            });
        }).catch((error) => {
            console.log(error);
        });
    }

    logout() {
        if (this.isUserLoggedIn()) {
            signOut(this.auth);
        }   
    }

    #fetchMotorPaths() {
        if (this.isUserLoggedIn()) {
            this.dataLeftMotorPath = `users/${this.#uid}/leftmotor`;
            this.dataRightMotorPath = `users/${this.#uid}/rightmotor`;
            this.connectedRobotID = `users/${this.#uid}/robotID`;
        }
    }

    #fetchMotorsValues() {
        if (this.isUserLoggedIn()) {
            let leftMotorFetch;
            let rightMotorFetch;
            get(child(ref(this.db), `users/${this.#uid}/leftmotor`)).then((snapshot) => {
                leftMotorFetch = snapshot.val();
              });
            get(child(ref(this.db), `users/${this.#uid}/rightmotor`)).then((snapshot) => {
                rightMotorFetch = snapshot.val();
            });  
            this.motorValues = {
                leftMotor: leftMotorFetch,
                rightMotor: rightMotorFetch,
            }
        }   
    }

    getUserPath() {
        if (this.isUserLoggedIn()) {
            return `users/${this.#uid}`;
        }
        else {
            throw new Error("User not logged in, no path available");
        }
    }
}