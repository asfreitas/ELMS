import React, { Component } from "react"
import axios from 'axios'

const Vehicle = props => (
    <div className="vehicle_status_column">
        <div className="vehicle_card">
            <div className="latitude">
                <p>Latitude</p>
                {props.vehicle.new_latitude}
            </div>
            <div className="unit">
                <p>Unit</p>
                {props.vehicle.vehicle_unit}
            </div>
            <div className="longitude">
                <p>Longitude</p>
                {props.vehicle.new_longitude}
            </div>
            <div className="active_time">
                Inactive time: 
                {props.getActiveTime(props.vehicle)}
            </div>
            <div className="nearest_vehicle">
                Nearest vehicle: 
                {props.getNearestVehicle(props.vehicle)}
            </div>
        </div>
    </div>
)

export default class VehicleInactive extends Component{
    constructor(props){
        super(props)
        this.getInactiveVehicles = this.getInactiveVehicles.bind(this);
        this.getNearestVehicle = this.getNearestVehicle.bind(this);
        this.getActiveTime = this.getActiveTime.bind(this);
        this.state={vehicles: []}
    }
    componentDidMount(){
        axios.get('https://elms-base-application.uc.r.appspot.com/vehicles/')
        .then(res => {
            this.setState({vehicles: res.data})
        })
        .catch((err) => {
            console.log(err);
        })
    }

    //set nearest vehicle
    getNearestVehicle(vehicle){
        var nearest = 0;
        var closest_distance = 10000;
        var total_close_vehicles = vehicle.distance_to_vehicles.length;
        for(var j = 0; j < total_close_vehicles; j++){
            if(vehicle.distance_to_vehicles[j].distance < closest_distance){
                closest_distance = vehicle.distance_to_vehicles[j].distance;
                nearest = vehicle.distance_to_vehicles[j].vehicle_unit;
            }
        }
        return nearest;
    }

    //get inactive time
    getActiveTime(vehicle){
        
        var date1 = new Date();
        //var date1 = vehicle.startup_time;
        var date2 = vehicle.last_received_time;

        //Convert dates into javascript readable date format and
        //get difference between dates/times in milliseconds
        var diff_in_milliseconds = Math.abs(new Date(date1) - new Date(date2));
        
        //Get milliseconds, seconds, minutes, and hours
        var milliseconds = parseInt((diff_in_milliseconds % 1000) / 100);
        var seconds = Math.floor((diff_in_milliseconds / 1000) % 60);
        var minutes = Math.floor((diff_in_milliseconds / (1000 * 60)) % 60);
        var hours = Math.floor((diff_in_milliseconds / (1000 * 60 * 60)) % 24);

        //To format single digit hours with a "0" 
        hours = (hours < 10) ? "0" + hours : hours;
        minutes = (minutes < 10) ? "0" + minutes : minutes;
        seconds = (seconds < 10) ? "0" + seconds : seconds;

        return hours + ":" + minutes + ":" + seconds + "." + milliseconds;
    }

    //get list of inactive vehicles
    getInactiveVehicles(){
        const vehicle_list_length = this.state.vehicles.length;
        var inactiveVehicles = [];
        var j = 0;
        for(var i = 0; i < vehicle_list_length; i++){
            if(this.state.vehicles[i].status === "inactive"){
                inactiveVehicles[j] = this.state.vehicles[i];
                j++;
            }
        }
        console.log(inactiveVehicles);
        return inactiveVehicles.map(currentVehicle => {
            return <Vehicle vehicle={currentVehicle} getNearestVehicle={this.getNearestVehicle} getActiveTime={this.getActiveTime} key={currentVehicle._id}/>;
        })
    }

    render(){
        return(
            <div className="inactive">
                <hr className="inactive_line"/>
                    <p className="inactive_text">Inactive</p>
                <hr className="inactive_line"/>
                { this.getInactiveVehicles() }
            </div>
        )
    }
}

