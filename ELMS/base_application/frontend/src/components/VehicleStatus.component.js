/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the structure for displaying the vehicle status.
*/

import React, { Component } from "react"

import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"
import MainContent from "./MainContent.component.js"
import VehicleStatusTiles from "./vehicle_status/VehicleStatusTiles.component.js"


export default class VehicleStatus extends Component{

    render(){
        return(
            <div className="page-container">
                <Navbar />
                <MainContent />
                <VehicleStatusTiles />
                <Footer />
            </div>
        )
    }
}
