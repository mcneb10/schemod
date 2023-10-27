(define (println txt)
    (display txt)
    (newline))

(println (_ "Hello, World!"))

; Setup

(window-set-size 640 480)

(window-set-title (_ "Devious Scheme"))

(window-set-framerate-limit 60)

(info "Loading textures...")

(define render-states
    (make-renderstates-with-texture
        (load-texture-from-file "../assets/textures/201.jpg")))

(info "Loading fonts...")

(define font
    (load-font-from-file "../assets/fonts/genkai-mincho.ttf"))

(define top-text
    (make-text "Hello" font 30))

(define num
    0)

(info "Setting up events...")

(add-event-hook "text_entered"
    (lambda (key)
        (format #t "Key press: ~A ~%" key)
        (set! num (+ 1 num))))

(add-event-hook "closed"
    (lambda () (println "Goodbye")))

(add-event-hook "resized"
    (lambda (width height)
        (format #t "Window resized! Width: ~S Height: ~S ~%" width height)))

(info "Entering loop...")

; The main game loop
(add-event-hook "loop"
    (lambda ()
        (cond
            ((is-key-pressed key-A) (println "A")))
        (window-clear)
        (window-draw top-text render-states)
        (window-display)
        (set-text top-text (format #f "Hello #~S" num))
        (set! num (+ 1 num))
        ))

