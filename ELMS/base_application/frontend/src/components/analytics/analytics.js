/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the 
* analytics page.
* 
* Utilized code from this source for building and stylizing table: 
* https://dev.to/abdulbasit313/an-easy-way-to-create-a-customize-dynamic-table-in-react-js-3igg
*/

import React, { Component } from 'react'
import VehicleRow from "../vehicle_status/VehicleRow.component"
import axios from 'axios'

const Analytics = props => (
        <tr>
            <td>{props.table.vehicle_unit}</td>
            <td>{props.table.pastVelocityAvg}</td>
            <td>{props.table.max_speed}</td>
            <td>{props.table.startup_time}</td>
        </tr>

)
    
export class Table extends Component {
    constructor(props) {
        super(props)
        this.state = {table: []}
    }
    componentDidMount(){
        axios.get('https://elms-base-application.uc.r.appspot.com/vehicles/analytics')
        .then(res => {
            this.setState({table: res.data})
            console.log(res.data);

        })
        
        .catch((err) => {
            console.log(err);
        })
    }


     renderTableData() {
        console.log(this.state.table.max_speed)
        return this.state.table.map(currentVehicle => {
            return <Analytics table={currentVehicle} getAverageSpeed={this.pastVelocityAvg} max_speed={this.max_speed} startup_time={this.getActiveTime} key={currentVehicle._id}/>;
        })
     }

  
     render() {
        return (
           <div>
            <table id='vehicles'>
                <thead>
                    <tr>
                        <th>Vehicle #</th>
                        <th>Average Speed</th>
                        <th>Max Speed</th>
                        <th>Startup date</th>
                    </tr>
                    </thead>
                <tbody>
                    {this.renderTableData()}
                </tbody>
            </table>
            <VehicleRow />
           </div>
        )
     }
}
