import FirebaseTools from "./FirebaseTools"

const FIREBASE = FirebaseTools.getInstance();

export function SignInButton() {
    return (
        <button onClick={() => FIREBASE.login()}>Sign In</button>
    )
}

export function SignOutButton() {
    return FIREBASE.auth.currentUser && (
        <button onClick={() => FIREBASE.logout()}>Sign Out</button>
    )
}