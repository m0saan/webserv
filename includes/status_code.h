//
// Created by Khalid Eddib on 8/11/2021.
//

#ifndef __STATUS_CODE_H__
#define __STATUS_CODE_H__



namespace HTTP {
    enum StatusCodes {

        /* 1xx Informational status code */

        /*
         *
         * A 1xx Informational status code means that the server has received the request and is continuing the process.
         * A 1xx status code is purely temporary and is given while the request processing continues.
         * For most tasks you won't encounter these much, as it's not the final response to the request.
         */

        CONTINUE=100,
        SWITCHING_PROTOCOLS=101,
        PROCESSING=102,
        EARLY_HINTS=103,


        /* 2xx Successful status code */

        /*
         *
         * A 2xx Successful status code means that the request was successful and the browser has received the expected information.
         * This is generally the one you want to see, as it means that the request was a success and has been received,
         * understood and accepted it. As a website owner you should make sure that all pages and resources (images, videos, etc.)
         * all return a 2xx status code.
         * This means that browsers can reach it successfully and that your website visitors can see and use your website.
         *
         */


         OK=200,
         CREATED=201,
         ACCEPTED=202,
         NON_AUTHORITATIVE_INFORMATION=203,
         NO_CONTENT=204,
         RESET_CONTENT=205,
         PARTIAL_CONTENT=206,
         MULTI_STATUS=207,
         ALREADY_REPORTED=208,
         IM_USED=226,

        /* 3xx Redirection status code */

        /*
         *
         * /
         * A 3xx Redirection status code means that you have been redirected and the completion of the request requires further action.
         * Redirects are a natural part of the internet and you shouldn't be scared to have 3xx redirect status codes on your website.
         * A redirect means that the request was received successfully, but that the resource was found elsewhere.
         * If a webpage has changed path and you try to access it through the old path, your CMS will often redirect the user to the new path.
         * Ultimately the request will end in a 2xx success, but first it must go through the 3xx redirection.
         *

        MULTIPLE_CHOICES=300,
        MOVED_PERMANENTLY=301,
        FOUND=302,
        SEE_OTHER=303,
        NOT_MODIFIED=304,
        USE_PROXY=305,
        TEMPORARY_REDIRECT=307,
        PERMANENT_REDIRECT=308,

         / * 4xx Client Error status code */

        /*
         *
         * A 4xx Client Error status code means that the website or the page could not be reached and either the page is unavailable or the request contains bad syntax.
         * As a website owner you should do your best to avoid these, as it means your users will not find what they're looking for.
         * This can be either pages that are no longer found and are either temporarily or permanently gone.
         */


        BAD_REQUEST=400,
        UNAUTHORIZED=401,
        PAYMENT_REQUIRED=402,
        FORBIDDEN=403,
        NOT_FOUND=404,
        METHOD_NOT_ALLOWED=405,
        NOT_ACCEPTABLE=406,
        PROXY_AUTHENTICATION_REQUIRED=407,
        REQUEST_TIME_OUT=408,
        CONFLICT=409,
        GONE=410,
        LENGTH_REQUIRED=411,
        PRECONDITION_FAILED=412,
        PAYLOAD_TOO_LARGE=413,
        REQUEST_URI_TOO_LING=414,
        UNSUPPORTED_MEDIA_TYPE=415,
        REQUESTED_RANGE_NOT_SATISFIABLE=416,
        EXPECTATION_FAILED=417,
        IM_A_TEAPOT=418,
        MISDIRECTED_REQUEST=421,
        UNPROCESSABLE_REQUEST=422,
        LOCKER=423,
        FAILED_DEPENDENCY=424,
        UPGRADE_REQUIRED=426,
        PRECONDITION_REQUIRED=428,
        TOO_MANY_REQUESTS=429,
        REQUEST_HEADER_FIELDS_TOO_LARGE=431,
        CONNECTION_CLOSED_WITHOUT_RESPONSE=444,
        UNAVAILABLE_FOR_LEGAL_REASONS=451,
        CLIENT_CLOSED_REQUEST=499,


        /* 5xx Server Error status code */

        /*
         *
         * A 5xx Server error status code means that while the request appears to be valid, the server could not complete the request.
         * If you're experiencing 5xx server errors for your website, you should immediately look at your server.
         * If you're hosting your own server you'll need to start debugging to figure out why it is not responding properly.
         * If you're using an external hosting provider you'll need to reach out to them, so they can look at it.
         *
         */

        INTERNAL_SERVER_ERROR=500,
        NOT_IMPLEMENTED=501,
        BAD_GATEWAY=502,
        SERVICE_UNAVAILABLE=503,
        GATEWAY_TIME_OUT=504,
        HTTP_VERSION_NOT_SUPPORTED=505,
        VARIANT_ALSO_NEGOTIATES=506,
        INSUFFICIENT_STORAGE=507,
        LOOP_DETECTED=508,
        NOT_EXTENDED=510,
        NETWORK_AUTHENTICATION_REQUIRED=511,
        NETWORK_CONNECTION_TIMEOUT_ERROR=599,
    };
}

#endif // __STATUS_CODE_H__
