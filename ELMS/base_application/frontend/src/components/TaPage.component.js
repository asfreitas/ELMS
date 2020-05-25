/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the Teacher Assistant Page
* for database manipulation
*/

import React, { Component } from 'react';

import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"
import AddVehicle from './TaPage/AddVehicle.component'

export default class TaPage extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <main>TA Testing Page</main>
                <AddVehicle />
                <Footer />
            </div>
        )
    }
}
