import React from "react"


function validateLogin(){
    
    var username = document.getElementById("user_name").value;
    var password = document.getElementById("password").value;

    if(username == "User123" && password == "pass123"){
        alert("Login successfully");
        window.location = "/VehicleStatus"
    }else{
        alert("Invalid login attempt");
        return false;
    }

}



class Login extends React.Component{
    
    /*
    constructor(props){
        super(props);
        this.state={
            username:'',
            password:''
        }
        this.validateLogin = this.validateLogin.bind(this)
    }

     THIS IS FOR LOGIN VALIDATION THROUGH REACT. CHANGE STATE OF THINGS
    THEN VALIDATE THE STATE. I THINK IT IS FOR SECURITY REASONS.
    USE onSubmit={this.validateLogin} INSTEAD

    validateLogin(event){
        this.setState({
            [event.target.name]: event.taget.value
        });
    }
    */

    render() {
        return(
            <div className="login_parent">
                <div className="login_box">
                    <h2 className="login">Base Login</h2>
                    <form id="login_form" method="post" className="login_form" onSubmit={validateLogin}>
                        <label className="user_name">User Name: </label>
                        <input id="user_name" type="text" name="User Name" placeholder="User Name"/>
                        <label className="password">Password: </label>
                        <input id="password" type="password" name="Password" placeholder="************"/>
                        <input type="submit" value="Login" id="Submit" className="Submit"/>
                    </form>
                </div>
            </div>
        )
    }
}

export default Login