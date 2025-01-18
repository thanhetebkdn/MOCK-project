#include "controller/MetadataController.h"
#include "common/Enum.h"
#include <iostream>
#include <climits>

/* Sets the current metadata tag */
void MetadataController::setCurrentTag(TagLib::Tag* tag) {
    currentTag = tag;
}

/* Handles displaying metadata for a given file path */
void MetadataController::handleShowMetadata(const string& filepath) {
    if (filepath.empty()) {
        cerr << "Error: Invalid file path!" << endl;
        return;
    }

    currentFilePath = filepath;
    currentFileRef = TagLib::FileRef(filepath.c_str());
    if (currentFileRef.isNull()) {
        cerr << "Error: Unable to open file. Please check the file path!" << endl;
        return;
    }

    currentTag = currentFileRef.tag();
    if (!currentTag) {
        cerr << "Error: No metadata available in the file!" << endl;
        return;
    }

    if (!currentFileRef.audioProperties()) {
        cerr << "Error: No audio properties available for the file!" << endl;
        return;
    }

    /* Display the metadata */
    map<string, string> metadata = Metadata::convertTagToMap(currentTag, currentFileRef.audioProperties());
    MetadataView metadataView;
    metadataView.displayMetadata(metadata);
}

/* Handles various metadata editing actions based on user input */
void MetadataController::handleAction(int action) {
    if (!currentTag) {
        cerr << "Error: No metadata loaded to edit!" << endl;
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            cout << "\nSwitching to Media File View..." << endl;
            mediaFileController->scanAndDisplayMedia();
        return;
    }

    string newValue;
    int newYear;

    switch (action) {
        case ACTION_EDIT_TITLE: {
            /* Edit Title */
            cout << "Enter new title: ";
            getline(cin, newValue);
            currentTag->setTitle(TagLib::String(newValue));
            saveMetadata();
            break;
        }
        case ACTION_EDIT_ARTIST: {
            /* Edit Artist */
            cout << "Enter new artist: ";
            getline(cin, newValue);
            currentTag->setArtist(TagLib::String(newValue));
            cout << "Artist updated successfully." << endl;
            saveMetadata();
            break;
        }
        case ACTION_EDIT_ALBUM: {
            /* Edit Album */
            cout << "Enter new album: ";
            getline(cin, newValue);
            currentTag->setAlbum(TagLib::String(newValue));
            cout << "Album updated successfully." << endl;
            saveMetadata();
            break;
        }
        case ACTION_EDIT_GENRE: {
            /* Edit Genre */
            cout << "Enter new genre: ";
            getline(cin, newValue);
            currentTag->setGenre(TagLib::String(newValue));
            cout << "Genre updated successfully." << endl;
            saveMetadata();
            break;
        }
        case ACTION_EDIT_YEAR: {
            /* Edit Year */
            cout << "Enter new year: ";
            cin >> newYear;
            currentTag->setYear(newYear);
            cin.ignore();
            cout << "Year updated successfully." << endl;
            saveMetadata();
            break;
        }
        case ACTION_EDIT_TRACK: {
            /* Edit Track Number */
            cout << "Enter new track number: ";
            cin >> newYear;
            currentTag->setTrack(newYear);
            cin.ignore();
            cout << "Track number updated successfully." << endl;
            saveMetadata();
            break;
        }
        case ACTION_EXIT_METADATA_EDITING: {
            /* Exit Metadata Editing */
            cout << "Exiting Metadata Editing..." << endl;
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
                break;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            cout << "\nSwitching to Media File View..." << endl;
            mediaFileController->scanAndDisplayMedia();
            return;
        }
        default:
            cerr << "Invalid choice! Please try again." << endl;
            break;
    }

    currentFileRef = TagLib::FileRef(currentFilePath.c_str());
    if (currentFileRef.isNull()) {
        cerr << "Error: Unable to refresh metadata!" << endl;
        return;
    }

    /* Show updated metadata */
    handleShowMetadata(currentFilePath);
}

/* Saves the updated metadata to the file */
void MetadataController::saveMetadata() {
    if (currentFileRef.isNull()) {
        cerr << "Error: currentFileRef is null! Cannot save metadata." << endl;
        return;
    }

    if (currentFileRef.save()) {
        cout << "Metadata saved successfully!" << endl;
    } else {
        cerr << "Error: Could not save metadata to file." << endl;
    }
}
