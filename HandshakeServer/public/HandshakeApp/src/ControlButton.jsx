import { ref, update } from 'firebase/database';
import FirebaseTools from './FirebaseTools';

export default function ControlButton(props) {
    const name = props.name;
    const value = props.value;
    const pathRef = props.pathRef;
    const FIREBASE = FirebaseTools.getInstance();

    const updates = {};

    function handleButtonDown() {
        console.log("pressed");
        updates[pathRef.toString().substring(pathRef.root.toString().length-1)] = value;
        update(ref(FIREBASE.db), updates);
    }
    function handleButtonUp() {
        console.log("released");
        updates[pathRef.toString().substring(pathRef.root.toString().length-1)] = 95;
        update(ref(FIREBASE.db), updates);
    }

    return (
        <>
            <button 
                onMouseDown={handleButtonDown} 
                onMouseUp={handleButtonUp}
                onTouchStart={handleButtonDown}
                onTouchEnd={handleButtonUp}
            >{name}</button>
        </>
    )
}