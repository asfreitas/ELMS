/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the code for displaying the 
* analytics page.
*/

import React, { Component } from 'react'
import axios from 'axios'

const Analytics = props => (
        <tr>
            <td>{props.table.vehicle_unit}</td>
            <td>{props.table.pastVelocityAvg}</td>
            <td>{props.table.time_alive}</td>
            <td>{props.table.startup_time}</td>
        </tr>

)
    
export default class Table extends Component {
    constructor(props) {
        super(props)
        this.state = {table: []}
    }
    componentDidMount(){
        axios.get('http://localhost:8080/vehicles/analytics')
        .then(res => {
            this.setState({table: res.data})
            console.log(res.data);

        })
        
        .catch((err) => {
            console.log(err);
        })
    }
    convertTime(){
        var date, currentTime
        for(var j = 0; j < this.state.table.length; j++)
        {
            date = "";
            currentTime = this.state.table[j].time_alive
            if(currentTime / 86400000 > 0) // get days
            {
                date += Math.floor(currentTime  / 86400000) + " days "
                currentTime = currentTime % 86400000
            }
            if(currentTime / 3600000 > 0)
            {
                date += Math.floor(currentTime / 3600000) + " hours and "
                currentTime = currentTime % 3600000
            }
            if( currentTime / 60000 > 0)
            {
                date += Math.floor(currentTime / 60000) + " minutes"
            }
            this.state.table[j].time_alive = date
        }
    }

     renderTableData() {
        this.convertTime()

        return this.state.table.map(currentVehicle => {
            return <Analytics table={currentVehicle} getAverageSpeed={this.getAverageSpeed} startup_time={this.getActiveTime} key={currentVehicle._id}/>;
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
                        <th>Total Running Time</th>
                        <th>Startup time</th>
                    </tr>
                    </thead>
                <tbody>
                    {this.renderTableData()}
                </tbody>
            </table>
           </div>
        )
     }
}
