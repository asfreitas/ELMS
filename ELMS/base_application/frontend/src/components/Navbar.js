import React from "react"

function Navbar(){
    return(
        <div className="navbar" id="topnav">
            <ul className="topnav">
                <li className="nav"><a href="/home">Home</a></li>
                <li className="nav"><a href="/VehicleStatus">Vehicle Status</a></li>
                <li className="nav"><a href="/LatestCommunication">Latest Communications</a></li>
                <li className="nav"><a href="/Analytics">Analytics</a></li>
            </ul>
        </div>
    )
}

/*
var navContainer = document.getElementById("topnav");
var nav = navContainer.getElementsByClassName("nav");

for(var i = 0; i < nav.length; i++){
    nav[i].addEventListener("click", function(){
        var current = document.getElementsByClassName("active");
        if(current.length > 0){
            current[0].className.replace(" active", "");
        }
    });
}
*/
export default Navbar