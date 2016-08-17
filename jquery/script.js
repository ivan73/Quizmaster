// JavaScript Document
$(document).ready(function(e) {
    //GetSwitchState();
     
    $("#on").click(function(e) {       
        $("#off").removeAttr('checked');        
        TurnOn();
       });
    $("#off").click(function(e) {
        $("#on").removeAttr('checked');     
        TurnOff();
    });
    function TurnOn()   {
        $.get("/on", function(data, status){
            //alert("Data: " + data + "\nStatus: " + status);
            $("#switch_txt").html(data);
        });
    }
    function TurnOff()  {
        $.get("/off", function(data, status){
            //alert("Data: " + data + "\nStatus: " + status);
            $("#switch_txt").html(data);
        });
    }
});