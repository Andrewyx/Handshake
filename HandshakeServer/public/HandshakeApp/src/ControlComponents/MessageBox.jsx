import { ref, update } from "firebase/database";
import FirebaseTools from "../Util/FirebaseTools";
import { useState } from "react";


export default function MessageBox() {
    const [txt, setTxt] = useState("");
    const FIREBASE = FirebaseTools.getInstance();

    function sendMessage(e) {
        e.preventDefault();
  
        const updates = {};
        const formData = new FormData(e.target);
        const formJson = Object.fromEntries(formData.entries());
        updates[`robotIDs/${FIREBASE.connectedRobotID}/message`] = formJson.message;
        update(ref(FIREBASE.db), updates);
        setTxt("");
    }

    function updateMessage(e) {
        e.preventDefault();
        setTxt(e.target.value);
    }

    return (
        <form onSubmit={sendMessage}>
            <div>
                <textarea 
                    id="robotMessage" 
                    onChange={updateMessage}
                    name="message" 
                    value={txt} 
                    placeholder="Enter Message to Send" 
                />
                <button type="submit">Send</button>
            </div>
        </form>
    )
}
