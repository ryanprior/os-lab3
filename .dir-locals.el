;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((c++-mode
  (flycheck-clang-language-standard . "c++11")
  (flycheck-gcc-language-standard . "c++11")
  (eval setq-local cd-compile-directory
        (file-name-directory
         (car
          (dir-locals-find-file ".")))))
 (c-mode
  (mode . c++)))
