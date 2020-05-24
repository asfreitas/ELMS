/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the structure of the offline
* vehicles page.
*/

import React, { Component } from 'react';
import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"
import VehicleLastComm from "./last_comm/VehicleLastComm.component"



export default class LastCommunications extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <VehicleLastComm />
                <Footer />
            </div>
        )
    }
}
