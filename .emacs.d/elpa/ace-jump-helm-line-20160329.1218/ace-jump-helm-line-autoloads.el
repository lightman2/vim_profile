;;; ace-jump-helm-line-autoloads.el --- automatically extracted autoloads
;;
;;; Code:


;;;### (autoloads (ace-jump-helm-line-autoshow-mode ace-jump-helm-line-idle-exec-remove
;;;;;;  ace-jump-helm-line-idle-exec-add ace-jump-helm-line-and-select
;;;;;;  ace-jump-helm-line) "ace-jump-helm-line" "ace-jump-helm-line.el"
;;;;;;  (22285 57058 273763 615000))
;;; Generated autoloads from ace-jump-helm-line.el

(autoload 'ace-jump-helm-line "ace-jump-helm-line" "\
Jump to a candidate and execute the default action.

\(fn)" t nil)

(autoload 'ace-jump-helm-line-and-select "ace-jump-helm-line" "\
Jump to and select the candidate in helm window.

\(fn)" t nil)

(defalias 'ace-jump-helm-line-execute-action 'ace-jump-helm-line-and-select)

(autoload 'ace-jump-helm-line-idle-exec-add "ace-jump-helm-line" "\


\(fn FUNC)" nil nil)

(autoload 'ace-jump-helm-line-idle-exec-remove "ace-jump-helm-line" "\


\(fn FUNC)" nil nil)

(defvar ace-jump-helm-line-autoshow-mode nil "\
Non-nil if Ace-Jump-Helm-Line-Autoshow mode is enabled.
See the command `ace-jump-helm-line-autoshow-mode' for a description of this minor mode.
Setting this variable directly does not take effect;
either customize it (see the info node `Easy Customization')
or call the function `ace-jump-helm-line-autoshow-mode'.")

(custom-autoload 'ace-jump-helm-line-autoshow-mode "ace-jump-helm-line" nil)

(autoload 'ace-jump-helm-line-autoshow-mode "ace-jump-helm-line" "\
Automatically show line labels in `helm'.

\(fn &optional ARG)" t nil)

;;;***

;;;### (autoloads nil nil ("ace-jump-helm-line-pkg.el") (22285 57058
;;;;;;  279483 421000))

;;;***

(provide 'ace-jump-helm-line-autoloads)
;; Local Variables:
;; version-control: never
;; no-byte-compile: t
;; no-update-autoloads: t
;; coding: utf-8
;; End:
;;; ace-jump-helm-line-autoloads.el ends here
