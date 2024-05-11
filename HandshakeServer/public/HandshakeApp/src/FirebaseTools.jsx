import { initializeApp } from "firebase/app";
import { getDatabase } from 'firebase/database';

export default class FirebaseTools {
    // Initialize Firebase
    static app;
    static db;
    static #instance;
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
    }

    static getInstance() {
        if (this.#instance) {
            return this.#instance;
        } else {
            this.#instance = new FirebaseTools();
            return this.#instance;
        }
    }
}