import React, { Component } from "react"

import Navbar from './Navbar.component'
import Footer from "./Footer.component.js"
import MainContent from "./MainContent.component.js"
import VehicleStatusTiles from "./VehicleStatusTiles.component.js"


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
