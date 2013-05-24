(function () {
    "use strict";

    WinJS.UI.Pages.define("/pages/home/home.html", {

        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            // TODO: Initialize the page here.

            WinJS.Utilities.query("a").listen("click", this.linkClickEventHandler, false);

            // Retrieve the div that hosts the Rating control.
            var ratingControlDiv = document.getElementById("ratingControlDiv");

            // Retrieve the actual Rating control.
            var ratingControl = ratingControlDiv.winControl;

            // Register the event handler. 
            ratingControl.addEventListener("change", this.ratingChanged, false);

            // Retrieve the button and register our event handler. 
            var helloButton = document.getElementById("compute");
            helloButton.addEventListener("click", this.buttonClickHandler, false);

            // Retrieve the input element and register our
            // event handler.
            var ageInput = document.getElementById("ageImput");
            ageInput.addEventListener("change", this.nameInputChanged);

            var heightInput = document.getElementById("heightImput");
            heightInput.addEventListener("change", this.nameInputChanged);

            // Restore app data. 
            var roamingSettings = Windows.Storage.ApplicationData.current.roamingSettings;

            // Restore the user name.
           /* var userName =
                Windows.Storage.ApplicationData.current.roamingSettings.values["userName"];
            if (userName) {
                nameInput.value = userName;
            }

            // Restore the rating. 
            var greetingRating = roamingSettings.values["greetingRating"];
            if (greetingRating) {
                ratingControl.userRating = greetingRating;
                var ratingOutput = document.getElementById("ratingOutput");
                ratingOutput.innerText = greetingRating;
            }*/

            // If the app was terminated last time it ran, restore the personalized
            // greeting. 
            /*if (
                WinJS.Application.sessionState.previousExecutionState
                === Windows.ApplicationModel.Activation.ApplicationExecutionState.terminated) {
                var outputValue = WinJS.Application.sessionState.greetingOutput;
                if (outputValue) {
                    var greetingOutput = document.getElementById("greetingOutput");
                    greetingOutput.innerText = outputValue;
                }

            }*/

        },

        buttonClickHandler: function (eventInfo) {

            var age = document.getElementById("ageImput").value;
            var height = document.getElementById("heightImput").value
            var idealWeight = (height - (30.48 * 5)) * 0.91;
            var greetingString;
            if(height < 153){
                greetingString = "You are too short. Eat some Rastishka! :)";
            } else if (height > 300){
                greetingString = "you are allmost Dino!";
            } else {
                greetingString = "Woman: " + (idealWeight + 45.5) + "kg; Man: " + (idealWeight + 50) + "kg;";
            }
            
            document.getElementById("greetingOutput").innerText = greetingString;

            // Save the session data. 
            WinJS.Application.sessionState.greetingOutput = greetingString;
        },

        ratingChanged: function (eventInfo) {

            var ratingOutput = document.getElementById("ratingOutput");
            ratingOutput.innerText = eventInfo.detail.tentativeRating;

            // Store the rating for multiple sessions.
            var appData = Windows.Storage.ApplicationData.current;
            var roamingSettings = appData.roamingSettings;
            roamingSettings.values["greetingRating"] = eventInfo.detail.tentativeRating;
        },

        nameInputChanged: function (eventInfo) {
            var nameInput = eventInfo.srcElement;

            // Store the user's name for multiple sessions.
            var appData = Windows.Storage.ApplicationData.current;
            var roamingSettings = appData.roamingSettings;
            roamingSettings.values["userName"] = nameInput.value;
        },

        linkClickEventHandler: function (eventInfo) {
            eventInfo.preventDefault();
            var link = eventInfo.target;
            WinJS.Navigation.navigate(link.href);
        }


    });
})();
