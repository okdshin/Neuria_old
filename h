commit 88af08b386d0a70483db6368fc78cf0dd8f77759
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Sep 5 03:36:43 2012 +0900

    files were separated to directory.

commit f3483e28267434dd4682819c183ad65d3edc2896
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Sep 5 00:56:14 2012 +0900

    Utility added RandomRange.

commit 0afab1ed6268711acd583537bc94ced32ea5219b
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 22:29:22 2012 +0900

    DebugOutput was removed.

commit f66e32fca7fe4896d3a5d8c63dc3d342f707f634
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 22:27:42 2012 +0900

    LabeledSink was added. it enables user to output text with label.

commit d3391f0fe3814927a7be3c8243b2985c07da0f4c
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 14:34:32 2012 +0900

    Broadcast function does not use post function.

commit e8570a1df5bcb2620a8d87066f73cb4a096d4c30
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 14:23:58 2012 +0900

    Send function does not call service post.

commit 0576f0a298f34ef9a8138e801c0bb21009890886
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 14:14:33 2012 +0900

    Send function (Parmanently Send)was applied.

commit b4481d017f820532b10cf355b4dccbc3035a0eff
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Sep 4 12:35:01 2012 +0900

    SessionPool's Iterator integrated iterator.

commit 5925111d7cfbac7a56bf0855ec7b99e6ad7fed1f
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Sep 3 21:54:09 2012 +0900

    KeyHashDataBase was renamed KeyHashDb. KeyHash was added.

commit 0e87b0904bcfaa956cf734bd8051e5a51660db7c
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Sep 3 03:01:31 2012 +0900

    KeyHashDataBase uses SHA256 hash calcuration.

commit 792a9731c2fcc7b8591e938123f9b1e9911ff7a0
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Sep 3 02:23:54 2012 +0900

    KeyHashDataBase was added.

commit 31092d747578356d797049961e68e90d20cf0ba7
Author: s.okada <kokuzen@gmail.com>
Date:   Sun Sep 2 17:37:39 2012 +0900

    AcceptedManager removed some function and became FuncController.

commit beac7f1c191ecb0a769a75c6034f7d36671822cb
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 30 23:59:25 2012 +0900

    removed FuncManager because renamed one is existing.

commit 3e9b86e1c16a823ab8fc2145547abe5a08f5f9db
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 30 22:49:55 2012 +0900

    FuncManager was renamed AcceptedManager.

commit c32f1398efd686c4ce6562e9b58ee59efcca2010
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 30 22:19:26 2012 +0900

    SessionPool.log removed from git repo.

commit 07e48406c47ea5f19e00a10ceff79dd0973e68a6
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 30 22:17:08 2012 +0900

    FuncManager was fixed.

commit 2136df587998c5177a6411f445ce4394c689481d
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 30 18:36:15 2012 +0900

    P2pCore and Session receives customized functions. Session does not have Pool pointer.

commit efd67dd99389c63134ae7e8e8d706e9bba98105c
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:49:16 2012 +0900

    Node was removed because renamed one exists.

commit 1bcd5e12aa12f3103410adeb66b0a685081d902a
Merge: abfe530 eb7d675
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:48:28 2012 +0900

    Merge branch 'master' of github.com:okdshin/Neuria

commit abfe53018d4477304f7ebc78969424b1706c388f
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:46:42 2012 +0900

    Node was renamed to FuncManager.

commit 1800ee9f4f1e498fa80c2321762098571793b335
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:31:59 2012 +0900

    Node instance is always pointer.

commit eb7d67538e26a987555d45dbb44a425440d04c13
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:18:00 2012 +0900

    Update readme.md

commit b6bbf443ea3fb182ed95f4b1cf311df8a7e40ed5
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 01:02:57 2012 +0900

    P2pCore Sample App was changed little but function is same.

commit 2a174252986c6e11c4824802bab11f052b8cb156
Author: s.okada <kokuzen@gmail.com>
Date:   Wed Aug 29 00:34:12 2012 +0900

    enable to convert ByteArray <-> std::string.

commit 40d30694b88ef9434b5a8930389476d95fe5d81e
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 22:45:06 2012 +0900

    unit_test.py added.

commit caaf21377274a86991f5e9e644cdeeae38c4aa35
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 22:33:09 2012 +0900

    Removed RegisterFunc.

commit efa7c6f64885cb5d7dee3b4be0da4e21a4558191
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:54:38 2012 +0900

    Update readme.md

commit 80cac49c7dda4732617ea6095d4bb6bdcfc3b1cc
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:50:10 2012 +0900

    Update readme.md

commit b5d320f0fe5210dd274916ef180dd6d2fc3f464a
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:49:43 2012 +0900

    Update readme.md

commit 06bebcdcafd5776d545172d33a9465d36c262442
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:46:29 2012 +0900

    Update readme.md

commit ed9458413c32d641f1edf67a1e98b15eee40154a
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:40:19 2012 +0900

    Update readme.md

commit f282e9335f9cb2e55b220827a08b777eb0ed061d
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:39:24 2012 +0900

    Update readme.md

commit 655e3764d19f77e2a19a8fa08309b2bbd3e2d1cb
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 10:37:28 2012 +0900

    added readme.md.

commit 17f86cb4e492d48d08d7672ae4d46c7def566e37
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 02:14:16 2012 +0900

    Debug preprocessor was killed.

commit fd0f81ac9e80cd70e83121e4a0e22ee94487a6f5
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 02:09:41 2012 +0900

    P2pCore sample app loop bug killed.

commit da6289023bfa6e5fe8933071fde5905e6397b43c
Author: s.okada <kokuzen@gmail.com>
Date:   Tue Aug 28 01:52:59 2012 +0900

    P2pCore sample got help command.

commit 52f8f492bf47bbefd4f7cc0b41979e8405ae067e
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Aug 27 23:22:39 2012 +0900

    some Node member func name was change.d

commit 12958ff3bbe7f017a2bb62b2e6db4c5e4400a88b
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Aug 27 02:39:41 2012 +0900

    Node was added. Node enable users to register some on_receive_func.

commit d1c5554a9b16c532df0271c84feaf9f928551c1b
Author: s.okada <kokuzen@gmail.com>
Date:   Mon Aug 27 01:10:44 2012 +0900

    session can receive any length string.

commit a06c3fac9d5557a605392bb3db82de63edc4813e
Author: s.okada <kokuzen@gmail.com>
Date:   Sun Aug 26 02:29:18 2012 +0900

    enable to set send/receive byte array buffer size.

commit 683bf345044c5589fd3454cbfad04725dc4a47d6
Author: s.okada <kokuzen@gmail.com>
Date:   Sun Aug 26 01:11:03 2012 +0900

    DebugPrint was removed.

commit 5fad74a85519c2b8b3686d93c9a36aebc09bf90c
Author: s.okada <kokuzen@gmail.com>
Date:   Sun Aug 26 00:57:02 2012 +0900

    do not use DebugPrint but use member std::ostream instance.

commit a5ac2288d8172113f114834768edef8f2d224c6c
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 21:29:31 2012 +0900

    Handle* -> On*
    DEBUG_PRINT was applied.

commit b8ad9b6797b6e2636e3d8701763e70dd4d3b4a4f
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 17:43:04 2012 +0900

    * Node func moved to P2pCore.
    * Bad Address Bug when broadcast was killed.

commit d359d4065e210ae5d21e99c8651c67dfe990de14
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 15:32:58 2012 +0900

    using test cui app function.

commit a7a933489edb9ec60c0041f3203f90fd75a18c91
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 15:32:20 2012 +0900

    test cui app became function.

commit 4a2b08836472223aa0c0d548174c9176c636e6a4
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 01:40:45 2012 +0900

    Node enables on_receive_func to use p2pcore function.

commit c95124cdd455737cfb2219dd46460282931d71aa
Author: s.okada <kokuzen@gmail.com>
Date:   Sat Aug 25 00:47:08 2012 +0900

    A P2pCore instance is always pointer.

commit 38d1eaa92db65805c1c105c17e27fd5f0b270c30
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 23:58:36 2012 +0900

    devided sessions to upper and lower.

commit dff94d0aa98b7fef1f6ee48dd9e792988a6864a7
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 09:46:13 2012 +0900

    P2pNodeCore was renamed to P2pCore.

commit 58e0e40368f18dd55d00f6a88b5f4b07bc99d17d
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:52:44 2012 +0900

    P2pNode was removed because renamed one is existing.

commit 24fe1f5919b196d571e4882da2a3213196839b4b
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:51:19 2012 +0900

    P2pNode was renamed to P2pNodeCore

commit 74502960631b4992df968accd696f4d5c3cf707c
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:47:55 2012 +0900

    Revert "P2pNode was renamed to P2pNodeCore."
    
    This reverts commit b141b20e7e7f30a0cf8043fc2ad3886921a88d51.

commit a48996966da20e60a54fe04945f0bd6c0c441314
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:47:39 2012 +0900

    added renamed file.

commit b141b20e7e7f30a0cf8043fc2ad3886921a88d51
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:44:27 2012 +0900

    P2pNode was renamed to P2pNodeCore.

commit 49eae5a80504e4157906d2ea5a1ff7c21047f002
Author: s.okada <kokuzen@gmail.com>
Date:   Fri Aug 24 00:39:20 2012 +0900

    Hash function applied. using openssl.

commit 80a9aaa3adb24ce8b392d279ed4da24d0c9735d9
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 23 19:38:11 2012 +0900

    user can custmize "on_receive_func".

commit 855314aa13a443e7d8fd032f6547b401c38ed29b
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 23 18:25:06 2012 +0900

    bad address error when reconnect and any other little bugs fixed.

commit d2342cbeb4f8660f31517d83a6a0f4ab6163327c
Author: s.okada <kokuzen@gmail.com>
Date:   Thu Aug 23 13:45:07 2012 +0900

    initial version. stable.
    using boost::asio
