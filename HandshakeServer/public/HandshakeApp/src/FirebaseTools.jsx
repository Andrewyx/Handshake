import { initializeApp } from "firebase/app";
import { child, get, getDatabase, onValue, ref, set } from 'firebase/database';
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
                        robotID : ""
                        });
                } 
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

    async #fetchMotorPaths() {
        if (this.isUserLoggedIn()) {
            onValue(ref(this.db, `users/${this.#uid}/robotID`), (snapshot) => {
                if (snapshot.exists()) {
                    this.connectedRobotID = snapshot.val();
                    this.#assignMotorsCurrentRobotID();
                }});
        }
    }

    #assignMotorsCurrentRobotID() {
        this.dataLeftMotorPath = `robotIDs/${this.connectedRobotID}/leftmotor`;
        this.dataRightMotorPath = `robotIDs/${this.connectedRobotID}/rightmotor`;
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