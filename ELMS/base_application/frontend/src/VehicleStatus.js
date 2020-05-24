/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the structure for the vehicle status page.
*/

import React from "react"

import Footer from "./components/Footer.js"
import Navbar from "./components/Navbar.js"
import MainContent from "./components/MainContent.js"
import VehicleStatusTiles from "./components/vehicle_status/VehicleStatusTiles.js"


function VehicleStatus(){
    return(
        <div className="page-container">
            <Navbar />
            <MainContent />
            <VehicleStatusTiles />
            <Footer />
        </div>
    )
}

export default VehicleStatus