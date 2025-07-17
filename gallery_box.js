// where gallery pictures will be displayed when the user click page buttons

//initialize image database, all are png files
const fall_24 = ['YanTingLeung_Skytrain','YanTingLeung_viffatwoodwards','YanTingLeung-bathroomstall','yantingleung-broccolihead','YanTingLeung-Centrismisntneutral','YanTingLeung-dininghall','YanTingLeung-Diwali','YanTingLeung-hallmarkermovie','YanTingLeung-waffles'];
const spring_25 = ['yantingleung - 101waystocheat','yantingleung - afro','yantingleung - anticlimatechange','yantingleung - brokensfu','yantingleung - lettertotrudeau','yantingleung - maplelecturehall','yantingleung - moderndating','yantingleung - new_lettertotrudeau','yantingleung - ngwssport','yantingleung - nianadurata','yantingleung - protest','yantingleung - snakerace','yantingleung - spoofclue','yantingleung - spoofgameoflife','yantingleung - transit','yantingleung - umbrella'];
const others = ['Lucy_oc','yantingleung - aiandjobs-1','yantingleung - aiandjobs-2','yantingleung - aiandjobs-3','yantingleung - jugojuice','yantingleung - xoxogossip-2'];

let image_list = "";

function renderPictures(button_msg){
    const gallery = document.querySelector(".js-gallery_grids");
    const fall24button = document.querySelector(".js-24_fall");
    const spring25button = document.querySelector(".js-25_spring");
    const otherbutton = document.querySelector(".js-others");
    image_list = "";
    if (button_msg === 'fall_2024'){
        for (i=0; i<fall_24.length;i++){
            image_list +=   `<div class="gallery_items">
                                <img src=\'peak_img/2024 Fall/${fall_24[i]}.png\' style="height: 400px; width: 300px; object-fit: scale-down; margin-left: 30px;">
                            </div>`;
        }
        //change button class
        fall24button.classList.add("gallery_button_selected");
        spring25button.classList.remove("gallery_button_selected");
        otherbutton.classList.remove("gallery_button_selected");
    }
    else if(button_msg === 'spring_2025'){
        for (i=0; i<spring_25.length;i++){
            image_list +=   `<div class="gallery_items">
                                <img src=\'peak_img/2025 Spring/${spring_25[i]}.png\' style="height: 400px; width: 300px; object-fit: scale-down; margin-left: 30px;">
                            </div>`;
        }
        //change button class
        fall24button.classList.remove("gallery_button_selected");
        spring25button.classList.add("gallery_button_selected");
        otherbutton.classList.remove("gallery_button_selected");
    }
    else if(button_msg === 'others'){
        for (i=0; i<others.length;i++){
            image_list +=   `<div class="gallery_items">
                                <img src=\'peak_img/${others[i]}.png\' style="height: 400px; width: 300px; object-fit: scale-down; margin-left: 30px;">
                            </div>`;
        }
        console.log("Other drawing");
        //Ruby!!!
        image_list +=   `<div class="gallery_items">
                                <img src=\'peak_img/ruby.gif\' style="height: 400px; width: 300px; object-fit: scale-down; margin-left: 30px;">
                            </div>`;
        //change button class
        fall24button.classList.remove("gallery_button_selected");
        spring25button.classList.remove("gallery_button_selected");
        otherbutton.classList.add("gallery_button_selected");
    }
    else{
        image_list += "Images not found :/";
    }
    gallery.innerHTML = image_list;
};