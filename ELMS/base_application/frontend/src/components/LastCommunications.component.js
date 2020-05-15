import React, { Component } from 'react';
import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"



export default class LastCommunications extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <p>Last Communications Components</p>
                <Footer />
            </div>
        )
    }
}