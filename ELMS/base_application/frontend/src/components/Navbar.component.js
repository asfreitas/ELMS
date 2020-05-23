/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the html structure for the 
* navigation bar.
*/

import React, { Component } from 'react';
import { Link } from 'react-router-dom';

export default class Navbar extends Component {
    render() {
        return (
            <nav className="navbar navbar-dark bg-dark navbar-expand-lg">
                <Link to="/" className="navbar-brand">Home</Link>
                <ul className="navbar-nav mr-auto">
                    <li className="navbar-item">
                        <Link to="/VehicleStatus" className="nav-link">Vehicle Status</Link>
                    </li>
                    <li className="navbar-item">
                        <Link to="/LastCommunications" className="nav-link">Last Communications</Link>
                    </li>
                    <li className="navbar-item">
                        <Link to="/Analytics" className="nav-link">Analytics</Link>
                    </li>
                    <li className="navbar-item">
                        <Link to="/AddVehicle" className="nav-link">Add Vehicle</Link>
                    </li>
                    <li className="navbar-item">
                        <Link to="/AddUser" className="nav-link">Add User</Link>
                    </li>
                </ul>
            </nav>
        )
    }
}