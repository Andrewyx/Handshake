import { ref, update } from 'firebase/database';
import FirebaseTools from '../Util/FirebaseTools';
import { hardwareConstants } from '../Util/util';

export default function ControlButton(props) {
    const name = props.name;
    const value = props.value;
    const path = props.path;
    const FIREBASE = FirebaseTools.getInstance();

    const updates = {};

    function handleButtonDown() {
        updates[path] = value;
        update(ref(FIREBASE.db), updates);
    }
    function handleButtonUp() {
        updates[path] = hardwareConstants.PWM_STOP_VALUE;
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