#include "bot.h"
#include "homework.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

#define MATCH_CMD(message, string)                                             \
  (memcmp(message, string, sizeof(string)) == 0)

static telebot_error_e handle_cmd(telebot_handler_t *handle,
                                  telebot_message_t *message) {
  if (MATCH_CMD(message->text, "/gettoday")) {
    time_t date = time(NULL);
    unsigned long now = date;
    homework_s *hm = homework_get_at(0);
    char *msg;
    if (!hm->description) {
      msg = "Nothing for today";
    } else {
      msg = hm->description;
    }
    return telebot_send_message(*handle, message->chat->id, msg, "HTML", false,
                                false, message->message_id, "");
  } else if (MATCH_CMD(message->text, "/gettomorrow")) {
    return telebot_send_message(*handle, message->chat->id,
                                "Tomorrow's homework", "HTML", false, false,
                                message->message_id, "");
  } else {
    return telebot_send_message(*handle, message->chat->id, "Unknown command",
                                "HTML", false, false, message->message_id, "");
  }

  return TELEBOT_ERROR_NONE;
}

void start_eventloop(telebot_handler_t *handle) {
  if (homework_init_db()) {
    return;
  }
  int index, count, offset = 0;
  telebot_error_e ret;
  telebot_message_t message;
  telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
  while (1) {
    telebot_update_t *updates;
    ret = telebot_get_updates(*handle, offset, 20, 5000, update_types, 0,
                              &updates, &count);
    if (ret != TELEBOT_ERROR_NONE)
      continue;
    printf("Number of updates: %d\n", count);
    for (index = 0; index < count; index++) {
      message = updates[index].message;
      if (message.text) {
        printf("%s: %s \n", message.from->first_name, message.text);
        ret = handle_cmd(handle, &message);
        if (ret != TELEBOT_ERROR_NONE) {
          printf("Failed to send message: %d \n", ret);
        }
      }
      // offset = updates[index].update_id + 1;
    }
    telebot_put_updates(updates, count);

    sleep(1);
  }
}
