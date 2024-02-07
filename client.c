#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
//#include "payload.c"
//#include "payload.h"


char* comp_reg_payload() {
    JSON_Value *json_value = json_value_init_object();
    JSON_Object *json_object = json_value_get_object(json_value);
    char username[30];
    char password[30];

    printf("username=");
    fgets(username, 30, stdin);
    char* aux = strchr(username, '\n');
    username[aux - username] = '\0';
    if(strchr(username, ' ') != NULL) {
        printf("E cringe rau sa ai username cu spatii, mai baga o fisa!\n");
        return "1";
    }
    
    printf("password=");
    fgets(password, 30, stdin);
    aux = strchr(password, '\n');
    password[aux - password] = '\0';
    if(strchr(password, ' ') != NULL) {
        printf("E cringe rau sa ai parola cu spatii, mai baga o fisa!\n");
        return "1";
    }

    json_object_set_string(json_object, "username", username);
    json_object_set_string(json_object, "password", password);

    return json_serialize_to_string(json_value);

}

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *status_msg;
    int sockfd;
    int status_code;
    char command[30];
    char *cookie;
    while (1) {
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    fgets(command, 30, stdin);
    command[strlen(command) - 1] = '\0';
        if (strcmp(command, "register") == 0) {

            char *payload = comp_reg_payload();
            if(strcmp(payload, "1") == 0) {
                continue;
            }
            message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register", 
            "application/json", &payload, 1, NULL, 0);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            sscanf(response, "%s%d%s", command, &status_code, message);
            if(status_code == 201) {
                
                printf("User created successfully!\n");
            } else {
                status_msg = basic_extract_json_response(response);
                printf("%s\nSe uita si la fata, mai incearca!\n", status_msg);
            }
        } else if(strcmp(command, "login") == 0) {
            char *payload = comp_reg_payload();
            if(strcmp(payload, "1") == 0) {
                continue;
            }
            message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login",
            "application/json", &payload, 1, NULL, 0);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            sscanf(response, "%s%d%s", command, &status_code, message);
            if(status_code == 200) {
                char* cookie_response = strstr(response, "connect.sid");
                cookie = strtok(cookie_response, ";");
                printf("Login realizat cu succes! Bine ai venit!\n");
            } else {
                status_msg = basic_extract_json_response(response);
                printf("%s\n", status_msg);
            }
        } else if(strcmp(command, "enter_library") == 0) {
            char *payload = NULL;
            message = compute_get_request("34.254.242.81", "/api/v1/tema/library/access",
            NULL, &cookie, 1);
            printf("%s\n", message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            sscanf(response, "%s%d%s", command, &status_code, message);
            if(status_code == 200) {
                printf("Acces la biblioteca realizat cu succes.\n");
            } else {
                status_msg = basic_extract_json_response(response);
                printf("%s\n", status_msg);
            }
        } else if (strcmp(command, "get_books") == 0) {
            char *payload = NULL;
            message = compute_get_request("34.254.242.81", "/api/v1/tema/library/books",
            NULL, &cookie, 1);

    }

   
    close_connection(sockfd);
    // free the allocated data at the end!

    return 0;
}
