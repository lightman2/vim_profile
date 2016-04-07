;;; rectangle-utils-autoloads.el --- automatically extracted autoloads
;;
;;; Code:


;;;### (autoloads (copy-rectangle rectangle-insert-at-right rectangle-menu
;;;;;;  extend-rectangle-to-end) "rectangle-utils" "rectangle-utils.el"
;;;;;;  (22168 45421 717327 679000))
;;; Generated autoloads from rectangle-utils.el

(autoload 'extend-rectangle-to-end "rectangle-utils" "\
Create a rectangle based on the longest line of region.

\(fn BEG END)" t nil)

(autoload 'rectangle-menu "rectangle-utils" "\


\(fn BEG END)" t nil)

(autoload 'rectangle-insert-at-right "rectangle-utils" "\
Create a new rectangle based on longest line of regionand insert string at right of it.
With prefix arg, insert string at end of each lines (no rectangle).

\(fn BEG END ARG)" t nil)

(autoload 'copy-rectangle "rectangle-utils" "\
Well, copy rectangle, not kill.

\(fn BEG END)" t nil)

;;;***

;;;### (autoloads nil nil ("rectangle-utils-pkg.el") (22168 45421
;;;;;;  728806 565000))

;;;***

(provide 'rectangle-utils-autoloads)
;; Local Variables:
;; version-control: never
;; no-byte-compile: t
;; no-update-autoloads: t
;; coding: utf-8
;; End:
;;; rectangle-utils-autoloads.el ends here
