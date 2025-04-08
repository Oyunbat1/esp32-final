#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>

// WiFi credentials
const char* ssid = "ESP32-Exam-System";
const char* password = "password123";

// Server
AsyncWebServer server(80);

// Admin and Customer credentials
String adminUsername = "admin";
String adminPassword = "admin123";
String customerUsername = "customer";
String customerPassword = "customer123";

const char* upload = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP File Upload</title>
</head>
<body>
    <h2>Upload File to ESP</h2>
    <form method="POST" action="/upload" enctype="multipart/form-data" id='upload_form'>
        <input type="file" name="file">
        <input type="submit" value="Upload">
    </form>
</body>
</html>
)";

const char* login = R"(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="login.css" />
    <title>Нэвтрэх хэсэг</title>
    <style>
      * {
        padding: 0;
        margin: 0;
        box-sizing: border-box;
      }
      :root {
        --background-color: #fff;
        --button-color: #0118d8;
        --button-color-hover: #4979ff;
        --button-color-active: #0a0f7c;
        --text-color: #fff;
        --text-color-hover: black;
        --font-family: "Roboto", Arial, sans-serif;
      }
      body {
        font-family: var(--font-family);
        background-image: url("../mubis_logo.webp");
        background-size: cover;
        background-repeat: no-repeat;
        background-position: center;
        height: 100vh;
        width: 100vw;
      }
      .container {
        backdrop-filter: blur(10px);
        height: 100vh;
        width: 100vw;
        display: flex;
        justify-content: center;
        align-items: center;
      }
      .container form {
        background-color: var(--background-color);
        width: 360px;
        height: auto;
        border-radius: 6px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.8);
        display: flex;
        flex-direction: column;
        align-items: center;
      }
      .container form .tabs-content {
        display: none;
      }
      .container form .tabs-content.active {
        display: block;
      }
      .container form .footer-button {
        width: 320px;
        height: 40px;
        padding: 6px;
        background-color: var(--button-color);
        color: var(--text-color);
        border-radius: 6px;
        border: none;
        outline: none;
        margin-bottom: 20px;
      }
      .container form .footer-button:hover {
        background-color: var(--button-color-hover);
        cursor: pointer;
      }
      .container form .footer-button:focus {
        background-color: var(--button-color);
        cursor: pointer;
      }
      .container form h1 {
        font-size: 24px;
        font-family: var(--font-family);
        font: bold;
        text-align: center;
        margin: 16px 0;
      }
      .container form .header-tabs {
        display: flex;
        background-color: var(--button-color);
        width: 320px;
        padding: 6px;
        border-radius: 6px;
        gap: 6px;
      }
      .container form .header-tabs button {
        width: 170px;
        height: 40px;
        border-radius: 6px;
        border: none;
        background-color: var(--button-color-hover);
        color: var(--text-color);
      }
      .container form .header-tabs button:hover {
        background-color: var(--background-color);
        cursor: pointer;
        color: var(--text-color-hover);
        transition: all 0.3s ease;
      }
      .container form .header-tabs button.active {
        background-color: white;
        color: black;
      }

      .container form .tabs-content .form-group {
        display: flex;
        flex-direction: column;
        gap: 22px;
        margin-top: 16px;
        position: relative;
      }
      .container form .tabs-content .form-group .upper,
      .container form .tabs-content .form-group .down {
        display: flex;
        flex-direction: column;
        gap: 3px;
      }
      .container form .tabs-content .form-group label {
        font-size: 12px;
        font-family: var(--font-family);
        color: #333;
      }
      .container form .tabs-content .form-group label span {
        color: red;
        font-size: 16px;
        margin-left: 5px;
      }
      .container form .tabs-content .form-group .upper input {
        padding: 10px;
        border-radius: 6px;
        width: 320px;
        height: 40px;
        border: 1px solid #9b9b9b;
      }
      .container form .tabs-content .form-group .down {
        margin-bottom: 30px;
      }
      .container form .tabs-content .form-group .down .input-pass-container {
        padding: 10px;
        border-radius: 6px;
        width: 320px;
        height: 40px;
        border: 1px solid #9b9b9b;
        display: flex;
        justify-content: space-between;
        align-items: center;
      }
      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container
        .eye-icon-container {
        border: 1px solid #9b9b9b;
        border-radius: 6px;
        padding: 4px;
        display: flex;
        align-items: center;
        justify-content: center;
      }
      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container
        .eye-icon-container
        img {
        width: 14px;
        height: 14px;
      }

      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container
        input {
        width: 340px;
        height: 30px;
        outline: none;
        visibility: none;
        border: none;
      }
      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container
        img {
        cursor: pointer;
      }

      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container:hover {
        outline: none;
        border-color: black;
      }
      .container form .tabs-content .form-group .upper input:hover {
        outline: none;
        border-color: black;
      }
      .container
        form
        .tabs-content
        .form-group
        .down
        .input-pass-container:focus {
        border-color: var(--button-color);
        outline: none;
      }

      .container form .tabs-content .form-group .upper input:focus {
        border-color: var(--button-color);
        outline: none;
      }
      .container form .tabs-content .form-group .upper input::placeholder {
        font-size: 12px;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <form id="login-form" action="/login" method="GET">
        <h1>Цахим шалгалтын систем</h1>
        <div class="header-tabs">
          <button
            type="button"
            class="tab-trigger active"
            onclick='showTab("customerTab")'
          >
            Хэрэглэгч
          </button>
          <button
            type="button"
            class="tab-trigger"
            onclick='showTab("adminTab")'
          >
            Админ
          </button>
        </div>
        <div id="customerTab" class="tabs-content active">
          <div class="form-group">
            <div class="upper">
              <label for="username">Нэвтрэх нэр <span>*</span></label>
              <input
                type="text"
                id="username"
                name="username"
                placeholder="Нэвтрэх нэрээ оруулна уу."
              />
            </div>
            <p
              id="errorMessage1"
              style="
                color: red;
                display: none;
                font-size: 10px;
                position: absolute;
                top: 64px;
                left: 8px;
              "
            >
              Та нэвтрэх нэрээ оруулна уу!
            </p>
            <div class="down">
              <label for="password">Нууц үг <span>*</span></label>
              <div class="input-pass-container">
                <input
                  type="password"
                  id="passwordCustomer"
                  name="password"
                  placeholder="Нууц үгээ оруулна уу."
                />
                <div class="eye-icon-container">
                  <img
                    onclick='togglePasswordVisibility("customer")'
                    id="eyeIconCustomer"
                    src="/eye-closed.webp"
                    alt="eye icon"
                  />
                </div>
              </div>
            </div>
            <p
              id="errorMessage2"
              style="
                color: red;
                display: none;
                font-size: 10px;
                position: absolute;
                top: 146px;
                left: 8px;
              "
            >
              Нууц үг оруулна уу!
            </p>
          </div>
        </div>

        <div id="adminTab" class="tabs-content">
          <div class="form-group">
            <div class="upper">
              <label for="adminUsername"
                >Нэвтрэх нэр (Админ)<span>*</span></label
              >
              <input
                type="text"
                id="adminUsername"
                name="adminUsername"
                placeholder="Нэвтрэх нэрээ оруулна уу."
              />
            </div>
            <p
              id="errorMessage1Admin"
              style="
                color: red;
                display: none;
                font-size: 10px;
                position: absolute;
                top: 64px;
                left: 8px;
              "
            >
              Та нэвтрэх нэрээ оруулна уу!
            </p>
            <div class="down">
              <label for="adminPassword">Нууц үг (Админ) <span>*</span></label>
              <div class="input-pass-container">
                <input
                  type="password"
                  id="adminPassword"
                  name="adminPassword"
                  placeholder="Нууц үгээ оруулна уу."
                />
                <div class="eye-icon-container">
                  <img
                    onclick='togglePasswordVisibility("admin")'
                    id="eyeIconAdmin"
                    src="/eye-closed.webp"
                    alt="eye icon"
                  />
                </div>
              </div>
            </div>
            <p
              id="errorMessage2Admin"
              style="
                color: red;
                display: none;
                font-size: 10px;
                position: absolute;
                top: 146px;
                left: 8px;
              "
            >
              Нууц үг оруулна уу!
            </p>
          </div>
        </div>

        <button class="footer-button" type="submit" id="HandleSubmit">
          Нэвтрэх
        </button>
      </form>
    </div>
    <script>
      const passwordCustomerInput = document.getElementById("passwordCustomer");
      const usernameInput = document.getElementById("username");

      const passwordAdminInput = document.getElementById("adminPassword");
      const adminUsernameInput = document.getElementById("adminUsername");

      const errorMessage1 = document.getElementById("errorMessage1");
      const errorMessage2 = document.getElementById("errorMessage2");
      const errorMessage1Admin = document.getElementById("errorMessage1Admin");
      const errorMessage2Admin = document.getElementById("errorMessage2Admin");

      const togglePasswordVisibility = (type) => {
        if (type === "customer") {
          const passwordInput = document.getElementById("passwordCustomer");
          const eyeIcon = document.getElementById("eyeIconCustomer");
          if (passwordInput.type === "password") {
            passwordInput.type = "text";
            eyeIcon.src = "/eye.webp";
          } else {
            passwordInput.type = "password";
            eyeIcon.src = "/eye-closed.webp";
          }
        } else if (type === "admin") {
          const passwordInput = document.getElementById("adminPassword");
          const eyeIcon = document.getElementById("eyeIconAdmin");
          if (passwordInput.type === "password") {
            passwordInput.type = "text";
            eyeIcon.src = "/eye.webp";
          } else {
            passwordInput.type = "password";
            eyeIcon.src = "/eye-closed.webp";
          }
        }
      };

      function showTab(tabID) {
        const tabs = document.querySelectorAll('.tabs-content');
        tabs.forEach((tab) => tab.classList.remove('active'));
        document.getElementById(tabID).classList.add('active');

        const triggers = document.querySelectorAll('.tab-trigger');
        triggers.forEach((trigger) => trigger.classList.remove('active'));

        if (tabID === "customerTab") {
          document
            .querySelector('.header-tabs button:nth-of-type(1)')
            .classList.add('active');
        } else {
          document
            .querySelector('.header-tabs button:nth-of-type(2)')
            .classList.add('active');
        }
      }

      const form = document.getElementById('login-form');
      form.addEventListener("submit", function (event) {
        let hasError = false;

        const activeTab = document.querySelector('.tabs-content.active');

        if (activeTab.id === "customerTab") {
          if (usernameInput.value === "") {
            event.preventDefault();
            errorMessage1.style.display = "block";
            hasError = true;
          } else {
            errorMessage1.style.display = "none";
          }

          if (passwordCustomerInput.value === "") {
            event.preventDefault();
            errorMessage2.style.display = "block";
            hasError = true;
          } else {
            errorMessage2.style.display = "none";
          }
        } else if (activeTab.id === "adminTab") {
          if (adminUsernameInput.value === "") {
            event.preventDefault();
            errorMessage1Admin.style.display = "block";
            hasError = true;
          } else {
            errorMessage1Admin.style.display = "none";
          }

          if (passwordAdminInput.value === "") {
            event.preventDefault();
            errorMessage2Admin.style.display = "block";
            hasError = true;
          } else {
            errorMessage2Admin.style.display = "none";
          }
        }

        if (hasError) {
          return;
        }
      });

      usernameInput.addEventListener("input", function () {
        if (usernameInput.value !== "") {
          errorMessage1.style.display = "none";
        }
      });

      passwordCustomerInput.addEventListener("input", function () {
        if (passwordCustomerInput.value !== "") {
          errorMessage2.style.display = "none";
        }
      });

      adminUsernameInput.addEventListener("input", function () {
        if (adminUsernameInput.value !== "") {
          errorMessage1Admin.style.display = "none";
        }
      });

      passwordAdminInput.addEventListener("input", function () {
        if (passwordAdminInput.value !== "") {
          errorMessage2Admin.style.display = "none";
        }
      });
    </script>
  </body>
</html>
)";

String customer = R"(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <title>Хэрэглэгчийн хэсэг</title>
  </head>
  <style>* {
    padding: 0;
    margin: 0;
    box-sizing: border-box;
  }
  :root {
    --background-color: #fff;
    --button-color: #0118d8;
    --button-color-hover: #4979ff;
    --button-color-active: #0a0f7c;
    --text-color: #fff;
    --text-color-hover: black;
    --font-family: "Roboto", Arial, sans-serif;
  }
  body {
    font-family: var(--font-family);
    background-image: url("../bg-image.webp");
    background-size: cover;
    background-repeat: no-repeat;
    background-position: center;
    height: 100vh;
    width: 100vw;
  }
  .container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    width: 100vw;
    backdrop-filter: blur(6px);
    position: relative;
  }
  .container .navbar {

position: absolute;
top: 0;
left: 0;
right: 0;
height: 90px;
}
.container .navbar .logo img{
width: 220px;
position: absolute;
top: 30px;
left: 40px;

}
  .container .sheet {
    display: flex;
    flex-direction: column;
    gap: 3px;
    position: absolute;
    top: 40px;
    right: 30px;
    cursor: pointer;
  }
  .container .sheet div{
    height: 4px;
    width: 30px;
    background-color: white;

  }
  .container .exam-pdf {
    background-color: yellow;
  }

    .sheet-overlay {
      position: fixed;
      top: 0;
      right: 0;
      width: 220px;
      height: 100%;
      background-color: rgba(70, 116, 244, 0.4);
      box-shadow: -2px 0 10px rgba(0, 0, 0, 0.2);
      padding: 20px;
      transform: translateX(100%);
      transition: transform 0.3s ease;
      z-index: 1000;
      backdrop-filter: blur(10px);
    }

    .sheet-overlay.open {
      transform: translateX(0);
    }
  .sheet-overlay  .close-btn {
      margin-top: 20px;
      background: #eee;
      border: none;
      padding: 12px 16px;
      border-radius: 100%;
      position: absolute;
      right: 27px;
      top: 10px;
      display: flex;
      justify-content: center;
      align-items: center;

    }
.sheet-overlay  .close-btn:hover {
    background-color: rgb(218, 218, 218);
  }
 .sheet-overlay .sheet-content {
    position: absolute;
    top: 100px;
    right: 0;
    left: 0;
    bottom: 0;

  }

 .sheet-overlay .sheet-content .exam-score {
    color: var(--text-color);
    font-size: 24px;
    width: 220px;
    display: flex;
    border-top: 1px solid #fff;
    border-bottom: 1px solid #fff;

  }
 .sheet-overlay .sheet-content .exam-score form button {
   border: none;
    width: 220px;
    height: 60px;
    background-color: var(--button-color-hover);
    color: white;
    
  }
  .sheet-overlay .sheet-content .exam-score form button:hover {
    background-color: var(--button-color);
    transition: all 0.3s ease;
    font: bold;
  }

  .container .main-content-container{
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    gap: 4px;
    border:  1px solid white;
    padding: 4px;
    border-radius: 6px;
    margin-top: 100px;
  }

  .container .main-content-container .exam-pdf{
    width: 340px;
    height: 300px;
    background-color: white;
    border-radius: 6px;
    padding: 10px;
    text-align: center;
  }
  .container .main-content-container .exam-pdf h1{
    font-size: 18px;
    padding-bottom: 6px;
  }
  .container .main-content-container .exam-pdf iframe{
    width: 100%;
    height: 256px;
    border-radius: 6px;

  }
  .container .main-content-container .form-container {
    width: 340px;
    background-color: white;
    border-radius: 6px;
    display: flex;
    justify-content: center;
  }

  .container .main-content-container .form-container form .questions-container{
    height: 170px;
    background-color: rgb(244, 244, 244);
    overflow-y: scroll;
    scrollbar-width: none;
    -ms-overflow-style: none;
    display: grid;
    grid-template-columns: repeat(2,1fr);
    gap: 10px;
    width: 300px;
    justify-content: center;
  padding-left: 36px;
  padding-top: 20px;
  padding-bottom: 30px;
    margin-left: 10px;
    margin-top: 4px;
  }

  .container .main-content-container .form-container form .questions-container .options-container{
display: flex;
flex-direction: column;
width: 100px;
text-align: center;
  }
  
  .container .main-content-container .form-container form .questions-container .options-container h3{
    font-size: 16px;
  }

  .container .main-content-container .form-container form .questions-container .options-container .question{
    display: flex;
    gap: 8px;
    margin-top: 10px;
  }

  .container .main-content-container .form-container form .questions-container .options-container .question .options{
    display: flex;
    flex-direction: column;
    border: 1px solid var(--button-color-hover);
    padding: 2px;
    border-radius: 4px;
  }
  .container .main-content-container .form-container form .questions-container .options-container .question .options:hover{
    border: 1px solid var(--button-color);
    background-color: #4979ff;
    color: #fff;
  }

  .container .main-content-container .form-container form .footer{
margin-bottom: 10px;
  }
  .container .main-content-container .form-container form .footer .last-input{
    display: flex;
    flex-direction: column;
    text-align: center;
    gap: 4px;
    margin-top: 4px;
  }
  .container .main-content-container .form-container form .footer .last-input input{
    width: 320px;
    height: 30px;
    padding: 2px;
    border: 1px solid #9b9b9b;
    border-radius: 6px;
    margin-top: 4px;
  }
  .container .main-content-container .form-container form .footer .last-input input:hover{
    border: 1px solid var(--button-color);
  }
  .container .main-content-container .form-container form .footer .last-input input:focus{
    border: 1px solid black;
    outline: none;
  }
  .container .main-content-container .form-container form .footer .btn{
    margin-top: 10px;
    width: 320px;
    height: 30px;
    padding: 6px;
    background-color: var(--button-color);
    color: var(--text-color);
    border-radius: 6px;
    border: none;
    outline: none;
    transition: all 0.3s ease-in-out;
  }
  .container .main-content-container .form-container form .footer .btn:hover{
    background-color: var(--button-color-hover);
    cursor: pointer;
    transition: all 0.3s ease-in-out;
  }
 
</style>
  <body>
    <div class="container">
      <div class="navbar">
        <div class="logo"><img src="/logo-white.webp" alt="logo"></div>
        <div class="sheet" onclick='openSheet()'>
          <div></div>
          <div></div>
          <div></div>
        </div>
      </div>

      <div class="main-content-container">
        <div class="exam-pdf">
          <h1>Шалгалт бөглөх хэсэг</h1>
          <iframe
            src="example.pdf"
            frameborder="0"
            type="application/pdf"
          ></iframe>
        </div>
        <div class="form-container">
          <form action="/customer" method="GET">
            <div class="questions-container">
              <div class="options-container">
                <h3>Асуулт 1</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q1" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q1" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q1" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q1" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 2</h3>
                <div class="question"><div class="options">
                  <label>A</label>
                  <input required type="radio" name="q2" value="A" />
                </div>
                <div class="options">
                  <label>B</label>
                  <input required type="radio" name="q2" value="B" />
                </div>
                <div class="options">
                  <label>C</label>
                  <input required type="radio" name="q2" value="C" />
                </div>
                <div class="options">
                  <label>D</label>
                  <input required type="radio" name="q2" value="D" />
                </div></div>
              </div>
              <div class="options-container">
                <h3>Асуулт 3</h3>
               <div class="question">
                <div class="options">
                  <label>A</label>
                  <input required type="radio" name="q3" value="A" />
                </div>
                <div class="options">
                  <label>B</label>
                  <input required type="radio" name="q3" value="B" />
                </div>
                <div class="options">
                  <label>C</label>
                  <input required type="radio" name="q3" value="C" />
                </div>
                <div class="options">
                  <label>D</label>
                  <input required type="radio" name="q3" value="D" />
                </div>
               </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 4</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q4" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q4" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q4" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q4" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 5</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q5" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q5" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q5" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q5" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 6</h3>
               <div class="question">
                <div class="options">
                  <label>A</label
                  ><input required type="radio" name="q6" value="A" />
                </div>
                <div class="options">
                  <label>B</label
                  ><input required type="radio" name="q6" value="B" />
                </div>
                <div class="options">
                  <label>C</label
                  ><input required type="radio" name="q6" value="C" />
                </div>
                <div class="options">
                  <label>D</label
                  ><input required type="radio" name="q6" value="D" />
                </div>
               </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 7</h3>
                <div class="question"><div class="options">
                  <label>A</label
                  ><input required type="radio" name="q7" value="A" />
                </div>
                <div class="options">
                  <label>B</label
                  ><input required type="radio" name="q7" value="B" />
                </div>
                <div class="options">
                  <label>C</label
                  ><input required type="radio" name="q7" value="C" />
                </div>
                <div class="options">
                  <label>D</label
                  ><input required type="radio" name="q7" value="D" />
                </div></div>
              </div>
              <div class="options-container">
                <h3>Асуулт 8</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q8" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q8" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q8" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q8" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 9</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q9" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q9" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q9" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q9" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 10</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q10" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q10" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q10" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q10" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 11</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q11" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q11" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q11" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q11" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 12</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q12" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q12" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q12" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q12" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 13</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q13" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q13" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q13" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q13" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 14</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q14" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q14" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q14" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q14" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 15</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q15" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q15" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q15" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q15" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 16</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q16" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q16" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q16" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q16" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 17</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q17" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q17" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q17" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q17" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 18</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q18" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q18" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q18" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q18" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 19</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q19" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q19" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q19" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q19" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 20</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q20" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q20" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q20" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q20" value="D" />
                  </div>
                </div>
              </div>
              
            </div>
    
            <div class="footer">
              <div class="last-input">
                <p>Нэр оруулах:
)";

String customer1 = R"(
</p>
                <input
                  type="text"
                  name="name"
                  placeholder="Нэрээ оруулна уу."
                />
              </div>
              <button class="btn" type="submit">Илгээх</button>
            </div>
          </form>
      </div>
        </div>
      </div>
    </div>
    <div id="sheet" class="sheet-overlay">
      <div class="sheet-content">
        <div class="exam-score">
          <form action="/data_harah" method="GET">
            <button type="submit" class="submit-btn">Шалгалтын дүн</button>
          </form>
        </div>
        <div class="exam-score">
          <form action="/zaawar.pdf" method="GET">
            <button type="submit" class="submit-btn">Шалгалтын заавар</button>
          </form>
        </div>
      </div>
      <button class="close-btn" onclick='closeSheet()'>X</button>
    </div>

    <script>
      function openSheet() {
        document.getElementById("sheet").classList.add("open");
      }
      function closeSheet() {
        document.getElementById("sheet").classList.remove("open");
      }
    </script>
  </body>
</html>
)";

const char* admin = R"(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Админы хэсэг</title>
  </head>
  <style>* {
    padding: 0;
    margin: 0;
    box-sizing: border-box;
  }
  :root {
    --background-color: #fff;
    --button-color: #0118d8;
    --button-color-hover: #235af0;
    --button-color-active: #0a0f7c;
    --text-color: #fff;
    --text-color-hover: black;
    --font-family: "Roboto", Arial, sans-serif;
  }
  body {
    font-family: var(--font-family);
    background-image: url("../bg-image.webp");
    background-size: cover;
    background-repeat: no-repeat;
    background-position: center;
    height: 100vh;
    width: 100vw;
  }
  .container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    width: 100vw;
    backdrop-filter: blur(6px);
    position: relative;
  }
  .container .navbar {

    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 90px;
  }
 .container .navbar .logo img{
  width: 220px;
  position: absolute;
  top: 30px;
  left: 40px;

 }

  .container .navbar .sheet {
    display: flex;
    flex-direction: column;
    gap: 3px;
    position: absolute;
    top: 40px;
    right: 30px;
    cursor: pointer;
  }
  .container .navbar .sheet div{
    height: 4px;
    width: 30px;
    background-color: white;

  }
  .container .exam-pdf {
    background-color: yellow;
  }

   .sheet-overlay {
    position: fixed;
    top: 0;
    right: 0;
    width: 220px;
    height: 100%;
    background-color: rgba(70, 116, 244, 0.4);
    box-shadow: -2px 0 10px rgba(0, 0, 0, 0.2);
    backdrop-filter: blur(10px);
    padding: 20px;
    transform: translateX(100%);
    transition: transform 0.3s ease;
    z-index: 1000;
  }

  .sheet-overlay.open {
    transform: translateX(0);
  }
.sheet-overlay  .close-btn {
    margin-top: 20px;
    background: #eee;
    border: none;
    padding: 12px 16px;
    cursor: pointer;
    border-radius: 100%;
    position: absolute;
    right: 27px;
    top: 10px;
    display: flex;
    justify-content: center;
    align-items: center;
  }
.sheet-overlay  .close-btn:hover {
    background-color: rgb(218, 218, 218);
  }
 .sheet-overlay .sheet-content {
    position: absolute;
    top: 100px;
    right: 0;
    left: 0;
    bottom: 0;
  }
 .sheet-overlay .sheet-content .exam-score {
    color: var(--text-color);
    font-size: 24px;
    margin-bottom: 20px;
    display: flex;
  width: 220px;
    flex-direction: column;
  }
  .sheet-overlay .sheet-content .exam-score form {
    border-bottom: 1px solid #fff;
    border-top: 1px solid #fff;
  }

 .sheet-overlay .sheet-content .exam-score form button {
   border: none;
    width: 220px;
    height: 60px;
    background-color: var(--button-color-hover);
    color: white;
    
  }
  .sheet-overlay .sheet-content .exam-score form button:hover {
    background-color: var(--button-color);
    transition: all 0.3s ease;
    font: bold;
  }

  .container .main-content-container{
    width: 352px;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    gap: 20px;
    border:  1px solid white;
    padding: 4px;
    border-radius: 6px;
    margin-top: 40px;
  }

  .container .main-content-container .header-form{
    background-color: white;
    width: 344px;
    height: 50px;
    border-radius: 6px;
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 0 8px;
  }
  .container .main-content-container .header-form  form{
    display: flex;
    gap: 6px;
    padding-left: 10px;
  }
  .container .main-content-container .header-form  form .first-input{
  width: 243px;
    height: 30px;
    background-color: var(--button-color);
    border-radius: 6px;
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 4px;
    color: white;
  }

  .container .main-content-container .header-form  form .first-input input[type="file"]{
    padding-left: 20px;
    border-radius: 6px;
  }

  .container .main-content-container .header-form  form .first-input:hover{
    background-color: var(--button-color-hover);
    transition: all ease-in-out 0.3s;
  }
  .container .main-content-container .header-form  form .second-input input:hover{
    background-color: var(--button-color-hover);
    transition: all ease-in-out 0.3s;
  }
  .container .main-content-container .header-form  form .second-input input{
    margin-right: 12px;
    height: 30px;
    width: 80px;
    border-radius: 6px;
    background-color: var(--button-color);
    color: white;
    border: none;
  }

  .container .main-content-container .form-container {
    width: 340px;
    background-color: white;
    border-radius: 6px;
    display: flex;
    justify-content: center;
  }

  .container .main-content-container .form-container form .questions-container{
    height: 300px;
    overflow-y: scroll;
    scrollbar-width: none;
    -ms-overflow-style: none;
    display: grid;
    grid-template-columns: repeat(2,1fr);
    gap: 10px;
    width: 300px;
    justify-content: center;
  padding-left: 36px;
  padding-top: 20px;
  padding-bottom: 30px;
    margin-left: 10px;
    margin-top: 4px;
  }

  .container .main-content-container .form-container form .questions-container .options-container{
display: flex;
flex-direction: column;
width: 100px;
text-align: center;
  }
  
  .container .main-content-container .form-container form .questions-container .options-container h3{
    font-size: 16px;
  }

  .container .main-content-container .form-container form .questions-container .options-container .question{
    display: flex;
    gap: 8px;
    margin-top: 10px;
  }

  .container .main-content-container .form-container form .questions-container .options-container .question .options{
    display: flex;
    flex-direction: column;
    border: 1px solid var(--button-color-hover);
    padding: 2px;
    border-radius: 4px;
  }
  .container .main-content-container .form-container form .questions-container .options-container .question .options:hover{
    border: 1px solid var(--button-color);
    background-color: #4979ff;
    color: #fff;
  }

  .container .main-content-container .form-container form .footer{

    margin:20px 0px;
  }
  .container .main-content-container .form-container form .footer .last-input{
    display: flex;
    flex-direction: column;
    text-align: center;
    gap: 4px;
  }
  .container .main-content-container .form-container form .footer .last-input input{
    width: 320px;
    height: 30px;
    padding: 2px;
    border: 1px solid #9b9b9b;
    border-radius: 6px;
    margin-top: 4px;
  }
  .container .main-content-container .form-container form .footer .last-input input:hover{
    border: 1px solid var(--button-color);
  }
  .container .main-content-container .form-container form .footer .last-input input:focus{
    border: 1px solid black;
    outline: none;
  }
  .container .main-content-container .form-container form .footer .btn{
    margin-top: 10px;
    width: 320px;
    height: 40px;
    padding: 6px;
    background-color: var(--button-color);
    color: var(--text-color);
    border-radius: 6px;
    border: none;
    outline: none;

    transition: all 0.3s ease-in-out;
  }
  .container .main-content-container .form-container form .footer .btn:hover{
    background-color: var(--button-color-hover);
    cursor: pointer;
    transition: all 0.3s ease-in-out;
  }
 
</style>
  <body>
    <div class="container">
      <div class="navbar">
        <div class="logo"><img src="/logo-white.webp" alt="logo"></div>
        <div class="sheet" onclick='openSheet()'>
          <div></div>
          <div></div>
          <div></div>
        </div>
      </div>

      <div class="main-content-container">
        <div class="header-form">
            <form
              method="POST"
              action="/upload"
              enctype="multipart/form-data"
              id="upload_form"
            >
              <div class="first-input"><input type="file" name="file" /></div>
              <div class="second-input">
                <input type="submit" value="Upload" />
              </div>
            </form>
          </div>
        <div class="form-container">
          <form action="/customer" method="GET">
            <div class="questions-container">
              <div class="options-container">
                <h3>Асуулт 1</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q1" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q1" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q1" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q1" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 2</h3>
                <div class="question"><div class="options">
                  <label>A</label>
                  <input required type="radio" name="q2" value="A" />
                </div>
                <div class="options">
                  <label>B</label>
                  <input required type="radio" name="q2" value="B" />
                </div>
                <div class="options">
                  <label>C</label>
                  <input required type="radio" name="q2" value="C" />
                </div>
                <div class="options">
                  <label>D</label>
                  <input required type="radio" name="q2" value="D" />
                </div></div>
              </div>
              <div class="options-container">
                <h3>Асуулт 3</h3>
               <div class="question">
                <div class="options">
                  <label>A</label>
                  <input required type="radio" name="q3" value="A" />
                </div>
                <div class="options">
                  <label>B</label>
                  <input required type="radio" name="q3" value="B" />
                </div>
                <div class="options">
                  <label>C</label>
                  <input required type="radio" name="q3" value="C" />
                </div>
                <div class="options">
                  <label>D</label>
                  <input required type="radio" name="q3" value="D" />
                </div>
               </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 4</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q4" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q4" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q4" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q4" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 5</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label>
                    <input required type="radio" name="q5" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label>
                    <input required type="radio" name="q5" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label>
                    <input required type="radio" name="q5" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label>
                    <input required type="radio" name="q5" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 6</h3>
               <div class="question">
                <div class="options">
                  <label>A</label
                  ><input required type="radio" name="q6" value="A" />
                </div>
                <div class="options">
                  <label>B</label
                  ><input required type="radio" name="q6" value="B" />
                </div>
                <div class="options">
                  <label>C</label
                  ><input required type="radio" name="q6" value="C" />
                </div>
                <div class="options">
                  <label>D</label
                  ><input required type="radio" name="q6" value="D" />
                </div>
               </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 7</h3>
                <div class="question"><div class="options">
                  <label>A</label
                  ><input required type="radio" name="q7" value="A" />
                </div>
                <div class="options">
                  <label>B</label
                  ><input required type="radio" name="q7" value="B" />
                </div>
                <div class="options">
                  <label>C</label
                  ><input required type="radio" name="q7" value="C" />
                </div>
                <div class="options">
                  <label>D</label
                  ><input required type="radio" name="q7" value="D" />
                </div></div>
              </div>
              <div class="options-container">
                <h3>Асуулт 8</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q8" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q8" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q8" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q8" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 9</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q9" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q9" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q9" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q9" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 10</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label
                    ><input required type="radio" name="q10" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label
                    ><input required type="radio" name="q10" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label
                    ><input required type="radio" name="q10" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label
                    ><input required type="radio" name="q10" value="D" />
                  </div>
                </div>
              </div>
              <div class="options-container">
                <h3>Асуулт 11</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q11" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q11" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q11" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q11" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 12</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q12" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q12" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q12" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q12" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 13</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q13" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q13" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q13" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q13" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 14</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q14" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q14" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q14" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q14" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 15</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q15" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q15" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q15" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q15" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 16</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q16" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q16" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q16" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q16" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 17</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q17" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q17" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q17" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q17" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 18</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q18" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q18" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q18" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q18" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 19</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q19" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q19" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q19" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q19" value="D" />
                  </div>
                </div>
              </div>
              
              <div class="options-container">
                <h3>Асуулт 20</h3>
                <div class="question">
                  <div class="options">
                    <label>A</label><input required type="radio" name="q20" value="A" />
                  </div>
                  <div class="options">
                    <label>B</label><input required type="radio" name="q20" value="B" />
                  </div>
                  <div class="options">
                    <label>C</label><input required type="radio" name="q20" value="C" />
                  </div>
                  <div class="options">
                    <label>D</label><input required type="radio" name="q20" value="D" />
                  </div>
                </div>
              </div>
              
            </div>
    
            <div class="footer">

              <button class="btn" type="submit">Илгээх</button>
            </div>
          </form>
      </div>
        </div>
      </div>
    </div>  
    <div id="sheet" class="sheet-overlay">
      <div class="sheet-content">
        <div class="exam-score">
          <form action="/push_data" method="GET">
            <button type="submit" class="submit-btn">Шалгалт дуусгах</button>
          </form>
          <form action="/data_harah" method="GET">
            <button type="submit" class="submit-btn">Шалгалтын дүн</button>
          </form>
            <form action="/zaawar.pdf" method="GET">
              <button type="submit" class="submit-btn">Шалгалтын заавар</button>
            </form>
        </div>
      </div>
      <button class="close-btn" onclick='closeSheet()'>X</button>
    </div>

    <script>
      function openSheet() {
        document.getElementById("sheet").classList.add("open");
      }
      function closeSheet() {
        document.getElementById("sheet").classList.remove("open");
      }
    </script>
  </body>
</html>
)";

File file;
String customertxt;

void Upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    static File file;
    if (!index) {
        Serial.printf("Uploading: %s\n", filename.c_str());
        file = LittleFS.open("/" + filename, "w");
    }
    if (file) {
        file.write(data, len);
    }
    if (final) {
        Serial.printf("Upload complete: %s (%d bytes)\n", filename.c_str(), index + len);
        file.close();
        request->send(200, "text/plain", "File upload successful");
    }
}

String grade(String admin, String custom, bool bl) {
  int cur = 0;
  String answer;
  for (int i = 0; i < admin.length(); i++) {
    if (admin[i] == custom[i]) cur++;
  }
  Serial.println(bl);
  if (bl == 1) {
    Serial.println(bl);
    answer = String(admin.length()) + "/" + String(cur);

    return answer;
  }
  else {
    return "0";
  }

  Serial.println();
}

void setup() {
  Serial.begin(115200);

  
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }
  

  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password, 1, 0, 10);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    //readFile(LittleFS, "/");
    listDir(LittleFS ,"/", 1);
    Serial.println("/");
    request->send(200, "text/html", login);
    Serial.println(readFile(LittleFS, "/admin.txt"));
    Serial.println(readFile(LittleFS, "/customer.txt"));
  });

  // Handle login
  server.on("/login", HTTP_GET, [](AsyncWebServerRequest* request) {
    listDir(LittleFS ,"/", 1);
    String username = request->arg("username");
    String password = request->arg("password");
    Serial.println(username);
    Serial.println(password);

    if (username == adminUsername && password == adminPassword) {
      //request->send(LittleFS, "/admin.html", "text/html");
      request->send(200, "text/html", admin);
    } else if (username == customerUsername && password == customerPassword) {
      //request->send(LittleFS, "/customer.html", "text/html");
      request->send(200, "text/html", customer + customer1);
    } else {
      request->send(401, "text/plain", "Invalid credentials!");
    }
  });

  // Admin page to set exam
  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("/admin");

    String a;
    String st;
    String admintxt = "";
    for (int i = 1; i <= 20; i++) {
      a = "a";
      a += String(i);

      Serial.println(a);
      st = request->arg(a);

      admintxt += st; 
    }

    writeFile(LittleFS, "/admin.txt", admintxt);

    request->send(200, "text/html", admin);
  });

  // Customer page to answer exam
  server.on("/customer", HTTP_GET, [](AsyncWebServerRequest* request) {
    String q;
    String st;
    String lastans;
    String admin = readFile(LittleFS, "/admin.txt");
    customertxt += request->arg("name") + "-";
    for (int i = 1; i <= 20; i++) {
      q = "q";
      q += String(i);

      Serial.println(q);
      st = request->arg(q);

      customertxt += st;
      lastans += request->arg(q);
    }

    customertxt += ",";

    request->send(200, "text/html", customer + grade(admin, lastans, 1) + customer1);
  });

  server.on("/push_data", HTTP_GET, [](AsyncWebServerRequest* request) {
    //Serial.println(customertxt);
    if (!customertxt.isEmpty()) writeFile(LittleFS, "/customer.txt", customertxt);
    customertxt = "";
    request->send(200, "text/html", admin);
  });

  /*
  <div style="display: flex; justify-content: center;align-items: center;height: 100vh;">
	<table border="1px" style="border: 1px solid #20cfa0; border-collapse: collapse ;">
		<tr><td style="padding: 8px;">aa</td><td style="padding: 8px;">aa</td><td style="padding: 8px;">aa</td><td style="padding: 8px;">aa</td></tr>
	</table>
  </div>
*/

  server.on("/data_harah", HTTP_GET, [](AsyncWebServerRequest* request) {
    String datas = readFile(LittleFS, "/customer.txt");
    String admin = readFile(LittleFS, "/admin.txt");
    String web;
    String name;
    String qs;
    bool bl = false;

    web += "<meta charset='UTF-8'>";
    web += "<div style='display: flex; justify-content: center;align-items: center;height: 100vh;'>";
    web += "<table border='1px' style='border: 1px solid #20cfa0; border-collapse: collapse ;'>";
    web += "<tr><td style='padding: 8px;'> Нэр </td><td style='padding: 8px;'>Таний хариулт</td><td style='padding: 8px;'>Шалгалтын хариу</td><td style='padding: 8px;'>Шалгалтын хувь</td></tr>";

    for (int i = 0; i < datas.length(); i++) {
      if (datas[i] == ',') {
        //Serial.println(name, qs);
        //Serial.print(name);
        //Serial.println(qs);
        web += "<tr><td style='padding: 8px;'>" + name + "</td><td style='padding: 8px;'>" + qs + "</td><td style='padding: 8px;'>" + grade(admin, qs, 1) + "</td><td style='padding: 8px;'>" + grade(admin, qs, 0) + "</td></tr>";
        name.clear();
        qs.clear();
        bl = false;

        continue;
      }

      if (!bl && datas[i] != '-') name += datas[i];
      else if (bl) qs += datas[i];
      
      if (datas[i] == '-') bl = true;

    }

    web += "</table></div>";

    request->send(200, "text/html", web);
  });

  server.on("/example.pdf", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/example.pdf", "application/pdf");
  });

  server.on("/example.pdf#zoom=5", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/example.pdf#zoom=5", "application/pdf");
  });

  server.on("/mubis_logo.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/mubis_logo.webp", "application/webp");
  });

  server.on("/eye.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/eye.webp", "application/webp");
  });

  server.on("/eye-closed.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/eye-closed.webp", "application/webp");
  });

  server.on("/bg-image.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/bg-image.webp", "application/webp");
  });

  server.on("/bg-image-2.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/bg-image-2.webp", "application/webp");
  });

  server.on("/logo-white.webp", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/logo-white.webp", "application/webp");
  });
  
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest* request) {
    listDir(LittleFS ,"/", 1);
    request->send(200, "text/html", admin);
  }, Upload);

  /*
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasParam("data", true, true) && LittleFS.exists(request->getParam("data", true, true)->value()))
      request->send(200, "", "UPLOADED: "+request->getParam("data", true, true)->value());
  });
  */
  
  /*
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {},
      [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                    size_t len, bool final) {handleUpload(request, filename, index, data, len, final);}
  );
  */

  
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/jquery.min.js", "text/javascript");
  });

    server.on("/A.txt", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/A.txt", "text/plain");
  });
  

    //writeFile(LittleFS, "/result.txt", "Hello ");
    //readFile(LittleFS, "/result.txt");

  server.begin();
}

void writeFile(fs::FS &fs, const char * path, String message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

/*
void wf(fs::FS &fs, const char * path, uint8_t *data){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.write(data)){
    listDir(LittleFS ,"/", 1);
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}
*/


void wf(fs::FS &fs, const char * path, uint8_t *data, size_t size){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.write(data, size)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return "";
  }

  String st;
  Serial.println("- read from file:");
  while(file.available()){
    st += file.readString();
    //Serial.println(file.readString());
    //Serial.write(file.read());
  }
  file.close();

  return st;
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void loop() {
  // Nothing to do here
}

