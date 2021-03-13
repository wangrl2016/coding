//
// Created by wangrl on 2021/3/13.
//

#include <string>
#include <thread>

enum CommandType {
    OPEN_NEW_DOCUMENT
};

struct UserCommand {
    CommandType type;

    UserCommand() : type(OPEN_NEW_DOCUMENT) {}
};

UserCommand getUserInput() {
    return UserCommand();
}

void openDocumentAndDisplayGUI(std::string const& filename) {}

bool doneEditing() {
    return true;
}

std::string getFilenameFromUser() {
    return "foo.doc";
}

void processUserInput(UserCommand const& command) {}

void editDocument(std::string const& filename) {
    openDocumentAndDisplayGUI(filename);
    while (!doneEditing()) {
        UserCommand command = getUserInput();
        if (command.type == OPEN_NEW_DOCUMENT) {
            std::string const newName = getFilenameFromUser();
            std::thread t(editDocument, newName);
            t.detach();
        } else {
            processUserInput(command);
        }
    }
}

int main(int argc, char** argv) {
    editDocument("bar.doc");
    return 0;
}