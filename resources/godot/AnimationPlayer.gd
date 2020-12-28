extends AnimationPlayer

# Settings
var imageDestination =  ProjectSettings.globalize_path("res://") + "animations/"
var finalSize = Vector2(300, 300)

# Objects
var defaultViewport
var timer 

# Runtime variables
var frame = 1
var currentAnimationName
var animations = get_animation_list()

# Called when the node enters the scene tree for the first time.
func _ready():
	defaultViewport = get_viewport()
	defaultViewport.set_transparent_background(true)
	print("Viewport bg transp: ", defaultViewport.transparent_bg)
	print("Viewport size: ", defaultViewport.size)
	print("Save renderings into path ", imageDestination)
	connect("animation_started", self, "_on_AnimationPlayer_animation_started")
	connect("animation_finished", self, "_on_AnimationPlayer_animation_finished")
	print("Available animations ", get_animation_list())
	
	timer = Timer.new()
	timer.connect("timeout",self,"_on_timer_timeout") 
	add_child(timer) 
	print("Ready")
	renderNextAnimation()


func _process(delta):
	pass


func renderNextAnimation():
	if animations.empty():
		print("Finished. All animations rendered")
		return
	
	# Take the next animation
	currentAnimationName = animations[0]
	animations.remove(0)
	print("Start rendering animation \"", currentAnimationName, "\" Animations left: ", animations)
	stop(true)
	set_current_animation(currentAnimationName) 
	timer.start(1)


func saveImage(image, name, index):
	image.convert(Image.FORMAT_RGBA8)
	image.flip_y()
	image.resize(finalSize.x, finalSize.y, Image.INTERPOLATE_LANCZOS)
	var finalName = name + "-right-%02d.png" % index
	print("Saving image ", finalName)
	image.save_png(imageDestination + finalName)
	image.flip_x()
	finalName = name + "-left-%02d.png" % index
	print("Saving image ", finalName)
	image.save_png(imageDestination + finalName)


func _on_timer_timeout():
	seek(frame, true)
	print("-->  ", frame, " ", current_animation_position, "/", current_animation_length)
	var image = defaultViewport.get_texture().get_data()
	saveImage(image, currentAnimationName, frame)
	frame += 1
	if (frame >= current_animation_length + 1):
		timer.stop()
		print("Done rendering ", currentAnimationName)
		renderNextAnimation()


func _on_AnimationPlayer_animation_started(animationName):
	print("Animation started ", animationName)


func _on_AnimationPlayer_animation_finished(animationName):
	print("Animation finished ", animationName)

