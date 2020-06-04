/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the Teacher Assistant Page
* for database manipulation
*/

import React, { Component } from 'react';
import Navbar from './Navbar.component'
import RemoveVehicles from './TaPage/RemoveVehicles.component'
import Footer from "./Footer.component.js"
import VehicleRow from '../components/vehicle_status/VehicleRow.component'

export default class TaPage extends Component {
    render(){
        return (
            <div>
                <Navbar />
                <main>TA Testing Page</main>
                <RemoveVehicles />
                <Footer />
            </div>
        )
    }
}
