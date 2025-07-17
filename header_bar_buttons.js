function go_home(){
    window.location="Ennil_website.html"
}

function to_aboutme(){
    window.location="aboutme.html";
}

function to_contact(){
    window.location="contact.html";
}

function to_gallery(){
    window.location="gallery.html";
}

function to_hobbies(){
    window.location="hobbies.html";
}

function to_plans(){
    window.location="plans.html";
}

//header bar expand and tuck function
let header_bar_tucked = false;




function renderHeaderBar(message){ //message = Welcome to my main page, About me, My hobbies, The Drawing Zone, Future Plans, Where to find me, etc
    console.log("headbar");
    if (header_bar_tucked) {
        const headerBar = document.querySelector('.js-headerbar_tucked');
        headerBar.innerHTML = `<div class="headbar_expand_button_box">
                <button style="font-size: 40px; 
                font-family: Arial; 
                font-weight: 700;
                background-color: orange;
                vertical-align: middle;
                height: 50px;
                width: 50px;
                border: none;"
                onclick="renderHeaderBar(\'${message}\')">
                v
                </button>
            </div>`;
        //headerBar.addEventListener("click",renderHeaderBar());
        headerBar.classList.add("js-headerbar_expand");
        headerBar.classList.remove("js-headerbar_tucked");
        header_bar_tucked = false;

    }
    else{
        const headerBar = document.querySelector('.js-headerbar_expand');
        headerBar.innerHTML = `
            <div class="headerbar_expand">

            <div class="logofeatures">
                <div class="logo_box">
                    <img class="logo" src="images/icon.png">
                    <p style="display: inline-flex; ">By ytl22</p>
                </div>

                <div class="features">
                    <h1 style="color: white; 
                    margin-left: 10px; 
                    margin-top: 50px; 
                    font-size: 48px;">
                        ${message}</h1>
                </div>
            </div>

            <div class="headerbarbuttonslot">
                <button class="headbarbutton" onclick="go_home()">Home</button>
                <button class="headbarbutton" onclick="to_aboutme()">About Me</button>
                <button class="headbarbutton" onclick="to_hobbies()">Hobbies</button>
                <button class="headbarbutton" onclick="to_gallery()">Gallery</button>
                <button class="headbarbutton" onclick="to_plans()">Plans</button>
                <button class="headbarbutton" onclick="to_contact()">Contact</button>
            </div>

            <div class="headbar_tuck_button_box">
                <button style="font-size: 40px; 
                font-family: Arial; 
                font-weight: 700;
                background-color: orange;
                height: 50px;
                width: 50px;
                border: none;"
                onclick="renderHeaderBar(\'${message}\')" >
                ^
                </button>
            </div>

        </div>`;
        headerBar.classList.add("js-headerbar_tucked");
        headerBar.classList.remove("js-headerbar_expand");
        header_bar_tucked = true;
    }
}