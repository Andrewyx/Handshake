import FirebaseTools from "./FirebaseTools"

const FIREBASE = FirebaseTools.getInstance();

export function SignInButton() {
    return (
        <button className="signButton" onClick={() => FIREBASE.login()}>Sign In</button>
    )
}

export function SignOutButton() {
    return FIREBASE.auth.currentUser && (
        <button  className="signOutButton"  onClick={() => FIREBASE.logout()}>Sign Out</button>
    )
}