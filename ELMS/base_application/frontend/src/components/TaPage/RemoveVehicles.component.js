/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for handling the add vehicle component.
*/

import React, { Component} from 'react';
import axios from 'axios'

//class used to add vehicle and information
export default class RemoveVehicles extends Component {
    constructor(props) {
        super(props);
        this.removeVehicles = this.removeVehicles.bind(this)

    }
    removeVehicles() {
        axios.delete('https://elms-base-application.uc.r.appspot.com/vehicles/deleteall')
        .then(res => {
            console.log(res.data);
        })
        
        .catch((err) => {
            console.log(err);
        })
    }



    //form used for creating a vehicle
    render(){
        return (
            <div className="deleteVehicle">    
                <div>
                    <h3 className="deleteText">Delete All Vehicles</h3>
                    <p className="taReminder">*Remember to delete vehicles.txt from C:\logs\saved_vehicles</p>
                </div>
                <div className="deleteButton">
                    <button type="submit" onClick={this.removeVehicles} className="btn btn-danger">Delete All</button>
                </div>
            </div>

        )
    }
}