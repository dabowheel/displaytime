DELETE FROM session WHERE expire < strftime("%Y-%m-%dT%H:%M:%S.%fZ");
