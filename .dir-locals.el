;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((prog-mode
  (flycheck-clang-language-standard . "c++11")
  (flycheck-gcc-language-standard . "c++11")
  (eval . (setq-local cd-compile-directory
                      (file-name-directory
                       (concat
                        (car
                         (dir-locals-find-file "."))
                        "build/")))))
 (c-mode
  (mode . c++)))
