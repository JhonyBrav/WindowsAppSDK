﻿#pragma once
/* ++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    MicrosoftTelemetry.h

Abstract:

    **************************************************************************
    IMPORTANT: Do NOT use this file for public samples or open-source
    projects. Instead, use:
    "minkernel/published/internal/telemetry/open_source/TraceLoggingConfig.h"
    **************************************************************************

    Microsoft Telemetry-specific definitions and extensions to logging functions:
        - Opt-in helpers to Microsoft Telemetry (TraceLoggingOptionMicrosoftTelemetry)
        - Keywords for categories (applies to TraceLogging and manifested events)
        - Event tags to influence persistence and latency
        - Field tags to influence PII treatment
        - Privacy data tag types
        - Targeting diagnostic data to specific products

    These should be used only by ETW providers in the Microsoft Telemetry provider group {4f50731a-89cf-4782-b3e0-dce8c90476ba}.

    Please see the following link for the full specification text:
    https://microsoft.sharepoint.com/teams/osg_threshold_specs/_layouts/15/WopiFrame.aspx?sourcedoc={8e8236cf-6b80-4e3c-9d7a-c35b52588946}&action=view

Environment:

    User mode or kernel mode.

--*/

#ifndef _MICROSOFTTELEMETRY_
#define _MICROSOFTTELEMETRY_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/*
Macro TraceLoggingOptionMicrosoftTelemetry():
Wrapper macro for use in TRACELOGGING_DEFINE_PROVIDER that declares the
provider's membership in the Microsoft Telemetry provider group
{B220FE06-1093-49EA-8220-6FCE05752645}. Membership in this group means that
events with keyword MICROSOFT_KEYWORD_TELEMETRY, MICROSOFT_KEYWORD_MEASURES,
or MICROSOFT_KEYWORD_CRITICAL_DATA will be recognized as "telemetry" events by
UTC.

    TraceLoggingOptionMicrosoftTelemetry()

is equivalent to:

    TraceLoggingOptionGroup(0xB220FE06, 0x1093, 0x49EA, 0x82, 0x20, 0x6F, 0xCE, 0x05, 0x75, 0x26, 0x45).

Example:

    TRACELOGGING_DEFINE_PROVIDER(g_hMyProvider, "MyProvider",
        (0xb3864c38, 0x4273, 0x58c5, 0x54, 0x5b, 0x8b, 0x36, 0x08, 0x34, 0x34, 0x71),
        TraceLoggingOptionMicrosoftTelemetry());
*/
#define TraceLoggingOptionMicrosoftTelemetry() \
    TraceLoggingOptionGroup(0xB220FE06, 0x1093, 0x49EA, 0x82, 0x20, 0x6F, 0xCE, 0x05, 0x75, 0x26, 0x45)

/*
Macro TraceLoggingOptionWindowsCoreTelemetry():
Wrapper macro for use in TRACELOGGING_DEFINE_PROVIDER that declares the
provider's membership in the Windows Core Telemetry provider group
{3E2F1611-BBF5-4AD6-9F10-F041360CDE0B}. Membership in this group means that
events with keyword MICROSOFT_KEYWORD_CRITICAL_DATA or event tag
MICROSOFT_EVENTTAG_CORE_DATA will be recognized as "telemetry" events by
UTC even at the Basic level.

    TraceLoggingOptionWindowsCoreTelemetry()

is equivalent to:

    TraceLoggingOptionGroup(0x3E2F1611, 0xBBF5, 0x4AD6, 0x9F, 0x10, 0xF0, 0x41, 0x36, 0x0C, 0xDE, 0x0B).

Example:

    TRACELOGGING_DEFINE_PROVIDER(g_hMyProvider, "MyProvider",
        (0xb3864c38, 0x4273, 0x58c5, 0x54, 0x5b, 0x8b, 0x36, 0x08, 0x34, 0x34, 0x71),
        TraceLoggingOptionWindowsCoreTelemetry());
*/
#define TraceLoggingOptionWindowsCoreTelemetry() \
    TraceLoggingOptionGroup(0x3E2F1611, 0xBBF5, 0x4AD6, 0x9F, 0x10, 0xF0, 0x41, 0x36, 0x0C, 0xDE, 0x0B)

/*
Privacy data tagging: Use TelemetryPrivacyDataTag(tag) in a telemetry
TraceLoggingWrite macro to indicate the type of data being collected.
Use the PDT macro values. If necessary, multiple tags may be OR'ed together,
e.g. TelemetryPrivacyDataTag(PDT_BrowsingHistory | PDT_ProductAndServiceUsage).

Typical usage:

    TraceLoggingWrite(
        g_hMyProviderHandle,
        "MyPerformanceEvent",
        TelemetryPrivacyDataTag(PDT_ProductAndServicePerformance),
        TraceLoggingKeyword(MICROSOFT_KEYWORD_MEASURES),
        TraceLoggingValue(MyPerformanceData));

Common PDT macros are defined below. Additional macros for advanced scenarios
are defined in MicrosoftTelemetryPrivacy.h.
*/
#define TelemetryPrivacyDataTag(tag) TraceLoggingUInt64((tag), "PartA_PrivTags")

#define PDT_BrowsingHistory                    0x0000000000000002u
#define PDT_DeviceConnectivityAndConfiguration 0x0000000000000800u
#define PDT_InkingTypingAndSpeechUtterance     0x0000000000020000u
#define PDT_ProductAndServicePerformance       0x0000000001000000u
#define PDT_ProductAndServiceUsage             0x0000000002000000u
#define PDT_SoftwareSetupAndInventory          0x0000000080000000u

/*
Targeting diagnostic data to a specific product: Use TelemetryTargetProduct(product) in a telemetry
TraceLoggingWrite macro to identify which products should emit the event. When a target product is
specified, the event will not be uploaded on other products that may exercise a common code path.
For example, if an event is added to a common OneCore component, but it is only applicable to Xbox,
a target product can be set to prevent the event from uploading on Desktop. Target product information
will also route documentation and compliance tasks to the proper privacy team for review.
Use the TTP macro values. If necessary, multiple products may be OR'ed together,
e.g. TelemetryTargetProduct(TTP_Xbox | TTP_Desktop). Typically this macro is not used, in which case
the event is applicable to all products on which it fires.

Example usage:

    TraceLoggingWrite(
        g_hMyProviderHandle,
        "MyXboxEvent",
        TelemetryPrivacyDataTag(PDT_ProductAndServicePerformance),
        TelemetryTargetProduct(TTP_Xbox),
        TraceLoggingKeyword(MICROSOFT_KEYWORD_MEASURES),
        TraceLoggingValue(MyPerformanceData));

Common TTP macros are defined below.
*/
#define TelemetryTargetProduct(product) TraceLoggingUInt64((product), "PartA_TargetProducts")

#define TTP_Xbox                               0x0000000000000001u
#define TTP_Edge                               0x0000000000000002u

/*
Telemetry categories that can be assigned as event keywords:

    MICROSOFT_KEYWORD_CRITICAL_DATA: Events that power user experiences or are critical to business intelligence
    MICROSOFT_KEYWORD_MEASURES:      Events for understanding measures and reporting scenarios
    MICROSOFT_KEYWORD_TELEMETRY:     Events for general-purpose telemetry

Only one telemetry category should be assigned per event, though an event may also participate in other non-telemetry keywords.

Some categories (such as CRITICAL_DATA) require formal approval before they can be used. Refer to
https://osgwiki.com/wiki/Common_Schema_Event_Overrides
for details on the requirements and how to start the approval process.
*/

// c.f. WINEVENT_KEYWORD_RESERVED_63-56 0xFF00000000000000 // Bits 63-56 - channel keywords
// c.f. WINEVENT_KEYWORD_*              0x00FF000000000000 // Bits 55-48 - system-reserved keywords
#define MICROSOFT_KEYWORD_CRITICAL_DATA 0x0000800000000000 // Bit 47
#define MICROSOFT_KEYWORD_MEASURES      0x0000400000000000 // Bit 46
#define MICROSOFT_KEYWORD_TELEMETRY     0x0000200000000000 // Bit 45
#define MICROSOFT_KEYWORD_RESERVED_44   0x0000100000000000 // Bit 44 (reserved for future assignment)

/*
Event tags that can be assigned to influence how the telemetry client handles events (TraceLogging only):

    MICROSOFT_EVENTTAG_CORE_DATA:                This event contains high-priority "core data".

    MICROSOFT_EVENTTAG_INJECT_XTOKEN:            Inject an Xbox identity token into this event.

    MICROSOFT_EVENTTAG_REALTIME_LATENCY:         Send these events in real time.
    MICROSOFT_EVENTTAG_COSTDEFERRED_LATENCY:     Treat these events like NORMAL_LATENCY until they've been stuck on the device for too long,
                                                    then allow them to upload over costed networks.
    MICROSOFT_EVENTTAG_NORMAL_LATENCY:           Send these events via the preferred connection based on device policy.

    MICROSOFT_EVENTTAG_CRITICAL_PERSISTENCE:     Delete these events last when low on spool space.
    MICROSOFT_EVENTTAG_NORMAL_PERSISTENCE:       Delete these events first when low on spool space.

    MICROSOFT_EVENTTAG_DROP_PII:                 The event's Part A will be reduced.
    MICROSOFT_EVENTTAG_HASH_PII:                 The event's Part A will be obscured.
    MICROSOFT_EVENTTAG_MARK_PII:                 The event's Part A will be kept as-is and routed to a private stream in the backend.
    MICROSOFT_EVENTTAG_DROP_PII_EXCEPT_IP:       The event's Part A will be reduced but the IP address will be stamped on the server.
    MICROSOFT_EVENTTAG_DROP_USER_IDS:            The event's Part A will not contain user identifiers.

    MICROSOFT_EVENTTAG_AGGREGATE:                The event should be aggregated by the telemetry client rather than sending each discrete event.

For example:

    TraceLoggingWrite(..., TraceLoggingEventTag(MICROSOFT_EVENTTAG_REALTIME_LATENCY), ...)

Some tags require formal approval before they can be used. Refer to
https://osgwiki.com/wiki/Common_Schema_Event_Overrides
for details on the requirements and how to start the approval process.

Note:
    Only the first 28 bits of the following event tag fields are allowed to be used. The rest will get dropped.
*/
#define MICROSOFT_EVENTTAG_DROP_USER_IDS            0x00008000

#define MICROSOFT_EVENTTAG_AGGREGATE                0x00010000

#define MICROSOFT_EVENTTAG_DROP_PII_EXCEPT_IP       0x00020000
#define MICROSOFT_EVENTTAG_COSTDEFERRED_LATENCY     0x00040000

#define MICROSOFT_EVENTTAG_CORE_DATA                0x00080000
#define MICROSOFT_EVENTTAG_INJECT_XTOKEN            0x00100000

#define MICROSOFT_EVENTTAG_REALTIME_LATENCY         0x00200000
#define MICROSOFT_EVENTTAG_NORMAL_LATENCY           0x00400000

#define MICROSOFT_EVENTTAG_CRITICAL_PERSISTENCE     0x00800000
#define MICROSOFT_EVENTTAG_NORMAL_PERSISTENCE       0x01000000

#define MICROSOFT_EVENTTAG_DROP_PII                 0x02000000
#define MICROSOFT_EVENTTAG_HASH_PII                 0x04000000
#define MICROSOFT_EVENTTAG_MARK_PII                 0x08000000

/*
Field tags that can be assigned to influence how the telemetry client handles fields and generates
Part A's for the containing event (TraceLogging only):

    MICROSOFT_FIELDTAG_DROP_PII: The field contains PII and should be dropped by the telemetry client.
    MICROSOFT_FIELDTAG_HASH_PII: The field contains PII and should be hashed (obfuscated) prior to uploading.

Note that in order to specify a field tag, a field description must be specified as well, e.g.:

    ..., TraceLoggingWideString(wszUser, "UserName", "User name", MICROSOFT_FIELDTAG_HASH_PII), ...
*/

#define MICROSOFT_FIELDTAG_DROP_PII 0x04000000
#define MICROSOFT_FIELDTAG_HASH_PII 0x08000000

#endif // _MICROSOFTTELEMETRY_
