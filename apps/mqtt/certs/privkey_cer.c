//#include "wiced_resource.h"
#include <certs/certs.h>
/*
const char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEogIBAAKCAQEArQZdcqeifkSJMgxldgw0P+O544hDzrnpldxK306Rj9xoRas0\n" \
"UmkyjUgkdkcCznrFT9esjjevvxjkXHag1+k7blW25fKt2ZEzHC5CCHR0FMWx2jHm\n" \
"mB50fH9STtsBUrkiWOwQZdvvcs7Gpv18Pdvu5CEIXp63EvMvcDyLWpYidAM5NZAq\n" \
"r6/7/BA7kmCr5LIBXM2+xPmocmgQS3hbsZAGCylXaKpYuV7tnDuXU50nymplj5m1\n" \
"p7nCKygQy1R+toPgwnlrAOVnRQMjZ0iPk2vxuyPJ1bANEIBiKykt64EAwPO0BgIK\n" \
"CxXMdYjKqibedrNLJ5/FBDPQllwhAB70Yv+fPwIDAQABAoIBAFIsZLMwRC79s77O\n" \
"Pl2bHDF/oOgb7kA+8V3X+sMlWGJfhjIYRkghZ/3LVm3J+d5AVnc98di1P8VdOA9l\n" \
"06vXO/+Bu9RntgNPP1vR6FCFph+7aGU9L8vtItuByIBk6LL19AcRThRiKM14wMXM\n" \
"O3ZRfppfyc7OSBo63rvKNVC3XvODUE0gKWSrBQyVi4wF0RtAFoNKHYqNpqMyOxtR\n" \
"S1CRu5UB4ybLT/RvZK1CQ9r4ZVVKyVFmKvKt38F95Btq1nQEixUyinZtq19HJ+/T\n" \
"hNRw1814WSzP3UeGb+z1dPej04ZUQ288N1SOABUYYfqu2ZCeZINZYgRTSG7qYytr\n" \
"g1vRUzECgYEA2+gf+zt/Dm2Sv64IS+ovfxm8tjlSR/cfGDSwIUhBJPYnqc4Vs4pl\n" \
"kP+CMTh3L9KNlqFEsPJ233Sm88p31AoMC6K30neGtsueWbpEEiu5b53Z835kx/Be\n" \
"ylg/sm7c2lELosHjPpUB/Bo58k5lMp+5fBJ2IjeU5ur6jNTolsUPt+kCgYEAyWxk\n" \
"l5tv2+an/TlOMQFu/juX2V7G9Xww3ANNYlPymlNMT0AaWe2AQeaNJ/tABcniZW/Y\n" \
"eV8MoW3KeXcUM8iyK8A17n7KpI6HAGmm4HuHvJ2URG4RhbIsxFmoTPdPHqnPwa+j\n" \
"mPqLf1UYKDz0p8XV4agX1ZDW96qmj3Dpq5pLzOcCgYAIwfvUE5CB0JdRjurBqzft\n" \
"BedgEnRWbV8NpbuF98ywXiVCO/TdS5uz5JkYwLjubKI6531G1sEpMQodF2kiRqwo\n" \
"InjTpvL0gLNzu1lOp/MgoNa3Pcdhtw7vXAhdYNDm2Odvue0E5wZHNfwszDZ3h6jp\n" \
"agrOXgxODfmgdjI1Hu/7UQKBgDG9irnFsMHlG+b0l9KSQA6YOM99EDTfDlHGnv2B\n" \
"LY/esiF/+wHqHJopjQIqkYzcZgQWsqaGSmAy9KU6Osru7NDRDoBu3yvz5nK7pHIb\n" \
"D1IMt+iOJdWWQmfanC0lhWxVEvEJpqTIelpfcS++ncwwlri7AefDdNKEWc1iXNH7\n" \
"sGxRAoGAK6afOypJqLzsPeW/1yxOLfExRy7koimn31p3A18SLArt6phhWIUvDF4K\n" \
"criSi9iDEu32y4qzZf1rrtj8JV88VdDg+HUeivbjEMdX23boVgI1HBlhYlOYYkv+\n" \
"48wDX0BUFGSzEBKk8ifG1uo388djGjLRp9gSXu/+ifiybzk6pwo=\n" \
"-----END RSA PRIVATE KEY-----\n";
*/
const char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAt1/B3lqz49pvoTPjo2N2dZW9WclBl7PMuxR6aMLM79nEg2ki\n" \
"BddlNjQKEVfayomiAVCBQDMxb4kXWXhVLBlrTlBD4RE1WubZPWBsRY9Pgvj30P2X\n" \
"NWIBiuO0KxKrlI1yOnOwbR85xNzKWYzAv+YKsFL3YfN8F/5/Lci3wDbdDflAl7Bf\n" \
"jwLOarAUmkFOjb0z6R2BZ0RYqOanlas5LYO+jHQF03JWvwPzTipI1mDUrf7TiNJ3\n" \
"qNn24XLURztGwfBeQnPM4VcXmd3D6zIPQBQxlDtC3Hle9LrLTGOvX8ip7ezjeT0b\n" \
"1126hsnvdJFmUalW+wJc9LlpgdgjPCJ+TokfEQIDAQABAoIBAEA1g4j8cakDP7tO\n" \
"lBMOLNiXiin6UsFv7+xCC2rLaz9BHZJ2Kbep6ILGThAslyv8bYHQUrHMOy827boi\n" \
"bJB4hh5x9JLGl8wwsTjQnpjsyYau7vd7FItYufmMKfY7H2Fw9eKJx+kt2eiY9zHM\n" \
"G2IWhv+ezYi63WT5dEbv1Ye0rkrJuJoAtiQ9kBcjJESBHBioAN8xdRj0zxucddvP\n" \
"/nM824E6Xs8j2plde7/zYP3fgXAhGZXSdUJzMAAhwILnHQTnZr8xlcabO9ia9FwL\n" \
"BWpaCrR6nGr55WzU/xgBKENG9ML/rqGUNeweOsXW9l8KoM+qMH3eHmDcCCfWZ0mG\n" \
"GpVwacECgYEA2xblXH2mLzJ57RW/sXVRzjx6dOaHiWUD461o/J6n4xFnwRte+M9D\n" \
"0/aM5NqfDh0NAXPzoYAJ4Vue7QTZntyDyUCZUgzCpsz1t7vJHwG5qG7J2H7vYK/S\n" \
"tFUO8XrMeBP5Z3jBSPL0OaIoQ4s5h9eIJj/SpEYgboyrkUazh+wh6g0CgYEA1kR+\n" \
"QmvcGufa0YikDj0ZI/+ywLTsu84WPk3YAmxfjYBtJMwJcPoOsMBalHlooJiuPitn\n" \
"hqG8Mx17AgaL9X6nTzBB3A+atH6m19qiQyMYZKlVNDhui7yw+0n4JZe1HEbet3hD\n" \
"g/7qkIKe8nttM0dzEjVsXF6EiCUo1OS6pCZlnBUCgYEAzPBF7Ja8dTnKQ7QsdFtC\n" \
"AqEgC2n5LtSHJtp3cpcWhJkrbL+lvyhiUkw2z3yRfCpAmDNWmqOimmOR7jSzPJ8Z\n" \
"x4/d25qEGnNvBkdqgs6iVKQNu65BTTgq6PUH3Wr5CPnmOpwCcbKSMbc14rjBQAil\n" \
"GuJor7HJdJ+xfVudZG/F6aECgYBnVG6jL4rc+tUe2UtshnB6WNRmdJn50+AMOdQj\n" \
"Dwphnf6EkVU4Q48UFB9anNYlJJxa5OmvFo5BBEWO64NRZ7vdx4LrUickRjPFwu0f\n" \
"fWxFFxg87733645aEQj3cBrqQQ8k4X2FyUhrWmK2w2kdIqbOsC7KuNYnJex0Br4v\n" \
"bUjxJQKBgQDaSEeabWRUYwj7Y0/ERaCzkVF47EZf7Zv0INFxCo9kNa3ZS9F/+cUU\n" \
"y2ygGHagUphcaNLQdqJ3gAYXMYitQl+ojoMH2uzktP4Yv3swxFOrgUkDv3/YASVN\n" \
"jXFxEcHGKn/zhTyMa3SgCzSG8nhlobcsjETgFQZRYHNeMSPJgOxAYQ==\n" \
"-----END RSA PRIVATE KEY-----\n";

const size_t privateKeySize = sizeof(privateKey);
