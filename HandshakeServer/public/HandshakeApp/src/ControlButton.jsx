import { ref, update } from 'firebase/database';
import FirebaseTools from './FirebaseTools';

export default function ControlButton(props) {
    const name = props.name;
    const value = props.value;
    const pathRef = props.pathRef;
    const FIREBASE = FirebaseTools.getInstance();

    const updates = {};

    

    function handleButtonDown() {
        updates[pathRef.toString().substring(pathRef.root.toString().length-1)] = value;
        update(ref(FIREBASE.db), updates);
    }
    function handleButtonUp() {
        updates[pathRef.toString().substring(pathRef.root.toString().length-1)] = 90;
        update(ref(FIREBASE.db), updates);
    }

    return (
        <>
            <button onMouseDown={handleButtonDown} onMouseUp={handleButtonUp}>{name}</button>
        </>
    )
}