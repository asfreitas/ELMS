/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the root code for the ELMS web application.
*/

import React from 'react';
import { BrowserRouter as Router, Route } from "react-router-dom";
import "bootstrap/dist/css/bootstrap.min.css"

import VehicleStatus from './components/VehicleStatus.component';
import Login from './components/Login.component';
import LastCommunications from './components/LastCommunications.component';
import Analytics from './components/Analytics.component';
import AddUser from './components/AddUser.component';
import TaPage from './components/TaPage.component'

function App() {
    return (
        <Router>
                <Route path='/' exact component={Login} />
                <Route path='/VehicleStatus' component={VehicleStatus} />
                <Route path='/LastCommunications' component={LastCommunications} />
                <Route path='/Analytics' component={Analytics} />
                <Route path='/AddUser' component={AddUser} />
                <Route path='/TaPage' component={TaPage} />
        </Router>
    )
}

export default App;