const route_input = document.getElementById("route-input");
const data_input = document.getElementById("data-input");
const send_button = document.getElementById("send-button");
const response_text = document.getElementById("response-text");

async function send_request() {
    let options = {
        method: document.querySelector('input[name="method-select"]:checked').value
    };

    if (["POST", "PUT"].includes(options.method)) {
        options.body = data_input.value;
    }

    const response = await fetch("/api/" + route_input.value, options);
    response_text.value = "";
    response_text.value = response.status + " " + response.statusText + "\n\n";

    for (const [key, value] of response.headers) {
        response_text.value += key + ": " + value + "\n";
    }
    response_text.value += "\n";

    response_text.value += await response.text();
}

send_button.addEventListener("click", () => {
    send_request();
});

document.addEventListener("keypress", e => {
    if (e.code == "Enter") {
        send_request();
        return false;
    }
})