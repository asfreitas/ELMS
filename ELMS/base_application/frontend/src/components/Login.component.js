import React, { Component } from "react"
import axios from 'axios';

export default class Login extends Component{
    
    constructor(props){
        super(props);

        this.onSubmitUsername = this.onSubmitUsername.bind(this);
        this.onSubmitPassword = this.onSubmitPassword.bind(this);
        this.validateLogin = this.validateLogin.bind(this);

        this.state={
            users: [],
            username:'',
            password:''
        }
        
    }

    //when page renders, get list of users in mongodb
    componentDidMount(){
        axios.get('https://elms-base-application.uc.r.appspot.com/users/')
            .then(res => {
                this.setState({ users: res.data})
                console.log(res.data);
            })
            .catch((err) => {
                console.log(err);
            })
    }


    //set username entered
    onSubmitUsername(event){
        this.setState({
            username: event.target.value
        })
    }

    //set password entered
    onSubmitPassword(event){
        this.setState({
            password: event.target.value
        })
    }


    validateLogin(event){
        event.preventDefault();

        const login_info = {
            username: this.state.username,
            password: this.state.password
        }

        console.log(login_info);

        const userListLength = this.state.users.length;
        for(var i = 0; i < userListLength; i++){
            if(this.state.username === this.state.users[i].username && this.state.password === this.state.users[i].password){
                alert("Login success!");
                window.location = '/VehicleStatus';
                break;
            }else{
                if(i === userListLength - 1){
                    alert("Invalid login attempt!");
                }
            }
        }
    }

    render() {
        return(
            <div className="login_parent">
                <div className="login_box">
                    <h2 className="login">Base Login</h2>
                    <form id="login_form" className="login_form" onSubmit={this.validateLogin}>
                        <label className="user_name">User Name: </label>
                        <input  id="user_name" 
                                type="text" 
                                name="User Name" 
                                placeholder="User Name" 
                                onChange={this.onSubmitUsername}/>
                        <label className="password">Password: </label>
                        <input  id="password" 
                                type="password" 
                                name="Password" 
                                placeholder="************"
                                onChange={this.onSubmitPassword}/>
                        <input type="submit" value="Login" id="Submit" className="Submit"/>
                    </form>
                    <div className="bottom_container">
                        <div className="row">
                            <div className="column">
                                <a href="/AddUser">Create profile</a>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        )
    }
}
