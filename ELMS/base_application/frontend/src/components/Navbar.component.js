/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for the navbar component.
*/

import React, { Component } from 'react';
import { Link } from 'react-router-dom';

export default class Navbar extends Component {
    render() {
        return (
            <nav className="navbar navbar-custom navbar-expand">
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
                        <Link to="/TaPage" className="nav-link">TA Page</Link>
                    </li>
                </ul>
            </nav>
        )
    }
}