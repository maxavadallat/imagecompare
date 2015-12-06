#ifndef CONSTANTS_H
#define CONSTANTS_H

// Application Info

#define DEFAULT_APPLICATION_NAME                        "ImageCompare"
#define DEFAULT_ORGANIZATION_NAME                       "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                     "mysoft.com"


// QML Context Properties

#define MAIN_VIEW_CONTROLLER                            "mainViewController"
#define COMPOSITOR_VIEW_CONTROLLER                      "compositorViewController"
#define SIDE_VIEW_CONTROLLER                            "sideViewController"
#define VIEWER_VIEW_CONTROLLER                          "viewerViewController"

#define DEFAULT_CUSTOM_COMPONENTS                       "customcomponents"
#define DEFAULT_CUSTOM_COMPONENT_COMPOSITOR             "Compositor"

#define CONTEXT_PROPERTY_SIDE                           "side"

#define CONTEXT_PROPERTY_SIDE_VALUE_LEFT                "left"
#define CONTEXT_PROPERTY_SIDE_VALUE_RIGHT               "right"

#define QML_SOURCE_SIDE_VIEW                            "qrc:/qml/ImageView.qml"
#define QML_SOURCE_COMPOSITE_VIEW                       "qrc:/qml/CompositeView.qml"
#define QML_SOURCE_VIEWER_VIEW                          "qrc:/qml/Viewer.qml"

// Settings Groups

#define SETTINGS_GROUP_MAIN                             "main"
#define SETTINGS_GROUP_UI                               "ui"


// Settings Keys

#define SETTINGS_KEY_VIEW_MAXIMIZED                     SETTINGS_GROUP_MAIN"/viewMaximized"

#define SETTINGS_KEY_LAST_DIR                           SETTINGS_GROUP_MAIN"/lastDir"
#define SETTINGS_KEY_LAST_FILE_LEFT                     SETTINGS_GROUP_MAIN"/lastFileLeft"
#define SETTINGS_KEY_LAST_FILE_RIGHT                    SETTINGS_GROUP_MAIN"/lastFileRight"

#define SETTINGS_KEY_MAIN_SPLITTER                      SETTINGS_GROUP_UI"/mainSplitter"
#define SETTINGS_KEY_LEFT_SPLITTER                      SETTINGS_GROUP_UI"/leftSplitter"
#define SETTINGS_KEY_RIGHT_SPLITTER                     SETTINGS_GROUP_UI"/rightSplitter"

#define SETTINGS_KEY_ZOOM_LEVEL                         SETTINGS_GROUP_MAIN"/zoomLevel"
#define SETTINGS_KEY_PAN_POSX                           SETTINGS_GROUP_MAIN"/panPosX"
#define SETTINGS_KEY_PAN_POSY                           SETTINGS_GROUP_MAIN"/panPosY"


#define SETTINGS_KEY_VIEWER_FULLSCREEN                  SETTINGS_GROUP_UI"/viewerFullScreen"

#define SETTINGS_KEY_VIEWER_POSX                        SETTINGS_GROUP_UI"/viewerPosX"
#define SETTINGS_KEY_VIEWER_POSY                        SETTINGS_GROUP_UI"/viewerPosY"
#define SETTINGS_KEY_VIEWER_WIDTH                       SETTINGS_GROUP_UI"/viewerWidth"
#define SETTINGS_KEY_VIEWER_HEIGHT                      SETTINGS_GROUP_UI"/viewerHeight"

#define SETTINGS_KEY_OPACITY_LEFT                       SETTINGS_GROUP_UI"/opacityLeft"
#define SETTINGS_KEY_OPACITY_RIGHT                      SETTINGS_GROUP_UI"/opacityRight"


// Supported Formats

#define DEFAULT_FILTER_TEMPLATE                         "*.%1"

#define DEFAULT_SUPPORTED_FORMAT_BMP                    "bmp"
#define DEFAULT_SUPPORTED_FORMAT_JPG                    "jpg"
#define DEFAULT_SUPPORTED_FORMAT_JPEG                   "jpeg"
#define DEFAULT_SUPPORTED_FORMAT_GIF                    "gif"
#define DEFAULT_SUPPORTED_FORMAT_PNG                    "png"

#define DEFAULT_SUPPORTED_FORMATS_FILTER                "*.png *.jpg *.jpeg *.bmp *.gif"



// Default Settings Values

#define DEFAULT_ZOOM_LEVEL_INDEX                        4
#define DEFAULT_ZOOM_LEVEL_INDEX_MAX                    6

const qreal zoomLevels[] = { 0.10, 0.25, 0.50, 0.75, 1.00, 2.00, 4.00, 8.00, 16.00, 100.00};


#define DEFAULT_STATUS_TEXT_SHOW_TIMEOUT                3000


#define TRANSFORM_ROTATE_LEFT                           -90
#define TRANSFORM_ROTATE_RIGHT                          90


enum FlipDirectionType
{
    FDTHorizontal   = 0,
    FDTVertical
};

enum RotationDirectionType
{
    RDTLeft         = 0,
    RDTRight
};

#define DEFAULT_TRANSFER_LIST_FILE_NAME                 ".transfers.list"

#define DEFAULT_TRANSFER_OPTIONS_YES_TO_ALL             0x0001
#define DEFAULT_TRANSFER_OPTIONS_NO_TO_ALL              0x0010


#define DEFAULT_COLOR_IMAGE_COMPARE_MATCH               qRgba(0, 200, 0, 50)
#define DEFAULT_COLOR_IMAGE_COMPARE_NOMATCH             qRgba(200, 0, 0, 50)

#define DEFAULT_MOUSE_WHEEL_PAN_STEP                    10

#endif // CONSTANTS_H

