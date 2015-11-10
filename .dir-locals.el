;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((c++-mode
  (eval setq-local cd-compile-directory
        (file-name-directory
         (car
          (dir-locals-find-file ".")))))
 (c-mode
  (mode . c++)))

