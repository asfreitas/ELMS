import React, { Component } from 'react';

import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"


export default class Analytics extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <p>Analytics Component</p>
                <Footer />
            </div>
        )
    }
}