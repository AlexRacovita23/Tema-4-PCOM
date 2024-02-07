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
#include "payload.h"


char* comp_reg_payload(char username[30], char password[30]) {
    JSON_Value *json_value = json_value_init_object();
    JSON_Object *json_object = json_value_get_object(json_value);
    size_t register_size = 30;
    //char *string = (char *) calloc (30, sizeof(char));

    printf("username=");
    getline(&username, &register_size, stdin);
    //char* username_end = strchr(username, '\n');
    username[strlen(username) - 1] = '\0';
    // username[username_end - username] = '\0';
    if(strchr(username, ' ') != NULL) {
        printf("E cringe rau sa ai username cu spatii, mai baga o fisa!\n");
        return "1";
    }

    printf("password=");
    getline(&password, &register_size, stdin);
    //char* password_end = strchr(username, '\n');
    username[strlen(password) - 1] = '\0';
    // username[username_end - username] = '\0';
    if(strchr(password, ' ') != NULL) {
        printf("E cringe rau sa ai parola cu spatii, mai baga o fisa!\n");
        return "1";
    }

    json_object_set_string(json_object, "username", username);
    json_object_set_string(json_object, "password", password);
    return json_serialize_to_string(json_value);

}