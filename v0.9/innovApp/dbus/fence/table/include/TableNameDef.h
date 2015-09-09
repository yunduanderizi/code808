#ifndef _TABLE_NAME_DEF_H_
#define _TABLE_NAME_DEF_H_

#define VDR_DRIVER_INFO_DB_NAME             ("drivers.db")
#define VDR_DRIVER_LOGIN_TABLE_NAME         ("login")

#define VDR_SETTING_DB_NAME                 ("vdrsetting.db")
#define VDR_RUN_PARA_TABLE_NAME             ("vdr")

#define VDR_DEPLOY_DB_NAME                  ("vdrdeploy.db")
#define VDR_DEPLOY_PARA_TABLE_NAME          ("deploy")

#define VDR_DEPLOY_STATUS_CONFIG_DB_NAME    ("vdrstatusconfig.db")
#define VDR_DEPLOY_STATUS_CONFIG_TABLE_NAME ("vdrstatusconfig")

#define VDR_ELECTRONIC_FENCE_DB_NAME        ("fence.db")
#define VDR_CIRCLE_FENCE_TABLE_NAME         ("circle")
#define VDR_RECTANGLE_FENCE_TABLE_NAME      ("rectangle")
#define VDR_POLYGON_FENCE_TABLE_NAME        ("polygon")
#define VDR_LINE_FENCE_TABLE_NAME           ("line")

#define VDR_MONITOR_DB_NAME                 ("monitor.db")
#define VDR_RECORD_INFO_TABLE_NAME          ("record")
#define VDR_CAPTURE_INFO_TABLE_NAME         ("capture")

#define VDR_TRACK_DB_NAME                   ("track.db")
#define VDR_ACCIDENT_RECORD_TABLE_NAME      ("accident")

#define VDR_TIRED_DB_NAME                   ("tired.db")
#define VDR_TIRED_DRIVING_TABLE_NAME        ("tired")

#define VDR_PRE_TIRED_DB_NAME               ("pretired.db")
#define VDR_PRETIRED_DRIVING_TABLE_NAME     ("pretired")

#define VDR_LOG_INFO_DB_NAME                ("loginfo.db")
#define VDR_PARAM_INFO_TABLE_NAME           ("paraminfo")
#define VDR_SPEED_INFO_TABLE_NAME           ("speedinfo")

#define VDR_OFF_STATUS_DB_NAME              ("offstatus.db")
#define VDR_OFF_STATUS_TABLE_NAME           ("offstatus")

#define VDR_DAY_INFO_DB_NAME                ("dayinfo.db")
#define VDR_DAY_INFO_TABLE_NAME             ("dayinfo")

#define VDR_TRACK_RUN_STATUS_DB_NAME        ("runstatus.db")
#define VDR_TRACK_RUN_STATUS_TABLE_NAME     ("runstatus")

#define VDR_LOCATION_STATUS_1_DB_NAME       ("locstatus1.db")
#define VDR_LOCATION_STATUS_1_TABLE_NAME    ("locstatus1")

#define VDR_LOCATION_STATUS_2_DB_NAME       ("locstatus2.db")
#define VDR_LOCATION_STATUS_2_TABLE_NAME    ("locstatus2")

#define VDR_CONTACTS_DB_NAME                ("vdrcontacts.db")
#define VDR_CONTACTS_TABLE_NAME             ("contacts")

#define VDR_PHONE_DB_NAME                   ("phone.db")
#define VDR_PHONE_PHONEBOOK_TABLE_NAME      ("phoneBook")
#define VDR_PHONE_SEND_SMS_TABEL_NAME       ("sendSms")
#define VDR_PHONE_RECEIVE_SMS_TABEL_NAME    ("receiveSms")
#define VDR_PHONE_CALL_RECORD_TABEL_NAME    ("callRecord")


#define VDR_DRIVER_LOGIN_TABLE_MAX_ROWS                 360
#define VDR_FENCE_CIRCLE_TABLE_MAX_ROWS                 100
#define VDR_FENCE_RECTANGLE_TABLE_MAX_ROWS              100
#define VDR_FENCE_POLYGON_TABLE_MAX_ROWS                100
#define VDR_FENCE_LINE_TABLE_MAX_ROWS                   100
#define VDR_MONITOR_RECORD_TABLE_MAX_ROWS               100
#define VDR_MONITOR_CAPTURE_TABLE_MAX_ROWS              100
#define VDR_TRACK_LOCATION_STATUS1_TABLE_MAX_ROWS       172800
#define VDR_TRACK_LOCATION_STATUS2_TABLE_MAX_ROWS       21600
#define VDR_TRACK_RUN_STATUS_TABLE_MAX_ROWS             100
#define VDR_TRACK_ACCIDENT_TABLE_MAX_ROWS               100
#define VDR_TRACK_TIRED_DRIVING_TABLE_MAX_ROWS          100
#define VDR_TRACK_PRETIRED_DRIVING_TABLE_MAX_ROWS       10
#define VDR_TRACK_OFF_STATUS_TABLE_MAX_ROWS             100
#define VDR_TRACK_DAY_INFO_TABLE_MAX_ROWS               100
#define VDR_TRACK_PARAM_INFO_TABLE_MAX_ROWS             100
#define VDR_TRACK_SPEED_INFO_TABLE_MAX_ROWS             10
#define VDR_CONTACTS_TABLE_MAX_ROWS                     100
#define VDR_PHONE_PHONEBOOK_TABLE_MAX_ROWS              (2000)
#define VDR_PHONE_RECIEVE_SMS_TABLE_MAX_ROWS			(1000)
#define VDR_PHONE_SEND_SMS_TABLE_MAX_ROWS				(1000)
#define VDR_PHONE_CALL_RECORD_TABLE_MAX_ROWS			(1000)

#endif
