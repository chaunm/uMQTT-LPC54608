//#include "wiced_resource.h"
#include <certs/certs.h>
/* AWS */
const char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEApRz6H/NlHuNJNGTmq9MMJJZrdLbsAGn1pATCGfvNDruAKrlj\n" \
"HSdq9MIdoqSqrVZDwCazV93S7FKhizfCSv5mHyUMaAtdDMrSXb0b1cTkAyHzDLkM\n" \
"C0OTCz3eRNS6Sl/epOBvSh1B+rJuUMueLe0vyFBLmvf0l2KMgoYzN1pchlzt5BID\n" \
"nY4GE+prXwtpx+oYuDIRbCe7koaoZ1eg9ghqp8zKHTwQzmL4RiRkjc36vtMxzo8r\n" \
"3PAUAYyy3BJQZUfydZw7jbojb19JH1uyhpUpZv2hkYJkBhnbkUzGaJQYcsNAd7Qi\n" \
"QmTvZf2Y+ce7h9NfvUYa4nyscfxH1hYY+JwSqQIDAQABAoIBAGACRX8mrHoVd4+7\n" \
"zFXFVp4/ySQI6ZokAu3oxmUNgeM7K/+r5PIi5ZKE/W6Bia1ZJ/QnjMle9zLrqXum\n" \
"ZJfu7brWdCxbzzONDF3thK382wjnSMjxIQu7s1kqFIC5ocl1jaUZzBywfpkj//mD\n" \
"LOtPrCUya4QlAli0uTd8iXN6W6rUl3UVxElw4fOtWJLvYroMgnDlakzMCJoLkCky\n" \
"lxGEz/fcA7p4MjTMDFQBUyFW6n3YEEByvdlhFxXN2MD7cc4/1nXlx5uONQL306D4\n" \
"gAK/CLG+e/qCz8T/TnTe/Eh0RtdfrJ/NA+NAHHehMK7p6f/HHSFeacUgjPz82oE4\n" \
"8r4rU3ECgYEAzvi1Acc2iZbOsxL2DEyxGPKiKzNfE/pNRR/n4mnlGgUdZEw98Yoh\n" \
"l3F/JCOEJxccbqyhRCGEES4B7gLBuW7QB4PrCSJ91WaJTjjXdULQgWvrnULnmwK1\n" \
"SYJaQcA8G1nY1NxtE/tv+EMtblkrFfgWzHs8aFf1JVHIJg0BDRNLdasCgYEAzDnf\n" \
"RMBaeRllDJbyQq5qwsq/yfBMC2EqFVkZYQ/+pyfUwE0cPW+jC/i/wJJrf01NfpTI\n" \
"YLs9rqx+A9DEu/rIm0lso5BOK5P337OfPPKERurfaDvNlTvvhi4ih2b2eoGyGUb/\n" \
"xOQElE5jA7DMM8c+n/AVm2iFofWni9u1HgNDHPsCgYBKxvSuxcjggQoBDPK0kujR\n" \
"OKh6rz7EuwQ720EuPlA1zG9IE3u1ztBAJRNofSR78w+ktN42/Vz30AWGdpyDYJPs\n" \
"gy7hyAJqnXcyObsudItclx8zXD7NdHcHLf9qDggpYZbDoTgPpiJYMf0yBylX8KBc\n" \
"FJJIqK67YwbLKJuem8ABrQKBgQCdYvQnfb27mdCAFUv6C4SHPcz0CVXxZx4RearP\n" \
"XI45r76LvGFJD2Gt9jOAatahMqfIJ00NDv+IP7KVqiwTWj+DPOCWZL15Y/0gzsKn\n" \
"K5a5OISXRY83tQrwc1l00+qbmpvwUwEFcVv+FzLDQ1r7hE9E3rcoHzLtD+3hIJ7B\n" \
"1G3CRQKBgF3eZ76ErikJ31szdFoAb6UJyV+5ztAoIA3u1GeRjXXKzfrefwiq9207\n" \
"vJtbasbwVQArbphS+ZNtSGKGZt7Xe+mBsfSOUhX4prBp34ZrANe+YnY6Jp9XDWXH\n" \
"nurYNjcbAsZJDXzUAmPB4QoVXLGsifllpUrk7oVTYHkZqoAM1b0T\n" \
"-----END RSA PRIVATE KEY-----\n";

/* raspberry pi
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
*/
const size_t privateKeySize = sizeof(privateKey);
