/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the structure for displaying
* the analytics page.
*/

import React, { Component } from 'react';

import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"
import Table from "./analytics/analytics"

export default class Analytics extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <Table /> 
                <Footer />
            </div>
        )
    }
}
